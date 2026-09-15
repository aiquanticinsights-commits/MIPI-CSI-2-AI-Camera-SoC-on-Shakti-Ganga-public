# Camera Connectivity Q&A – Shakti Ganga MIPI (v0.3-ganga-public)

> **Audience:** Systems / HW engineer evaluating the camera-to-Ganga path.
> **Goal:** answer three received queries with file:line evidence, actual-vs-target
> block diagrams, and fix actions. All file references are repo-relative.

## 0. Verdicts up front

| # | Query | Verdict | One-line answer | Trace |
|---|---|---|---|---|
| Q1 | How is the camera connected to Ganga? No MIPI signals in constraints | **Valid – by design of this release** | v0.3 bitstream is pattern-driven; no camera pads exist yet | `examples/arty_a7_100t/top_ganga_mipi.xdc`, `vivado/run_bitstream.tcl:24-25` |
| Q2 | MIPI_RX_SYSTEM IP seen in board files | **Clarified** | Xilinx `mipi_csi2_rx_subsystem:6.0` exists only in the unfinished `vivado/bd.tcl` skeleton, **not** in the verified bitstream | `vivado/bd.tcl:29`, `vivado/run_bitstream.tcl:21-38` |
| Q3 | No Shakti files visible | **Clarified** | No Shakti BSV/Verilog ships here; Ganga enters via prebuilt bitstream, pin-compatible constraints, and Ganga-addressed driver | `sw/linux_v4l2/shakti_mipi.dts`, release asset `ganga_mipi_top.bit` |

## 1. Q1 – Camera to Ganga connection (actual, v0.3)

> Flowchart `diagrams/camera_actual.mmd` (pattern source -> soft pipeline -> Arty pins)

![Camera actual – pattern-driven, no camera pads](diagrams/camera_actual.png)

*Pictorial keeps the same nets as flowchart: internal `tick_q/sot_phase`
drives `csi2_rx` LP/HS inputs -> `vc_merge` -> `isp_pipeline` ->
`ai_preproc`; only Arty pins are `E3 sys_clk`, `C2 sys_rst`,
`led[0..3]`, `D10/A9 uart` – zero MIPI lane pins.*

### Evidence

| Fact | Trace |
|---|---|
| Top has 7 external ports, none MIPI | `top_ganga_mipi` port list: `sys_clk`, `sys_rst`, `led[3:0]`, `uart_txd`, `uart_rxd` (top is commercial-RTL; port list in `docs/BITSTREAM_GANGA.md`) |
| CSI-2 LP/HS driven internally | Pattern block drives `lp_rx_p/n`, `hs_rx_p/n` from a tick counter |
| Constraints carry no MIPI | `examples/arty_a7_100t/top_ganga_mipi.xdc`: 9 constrained ports, all clk/rst/led/uart |
| Verified result still holds | XSIM PASS, WNS +5.534 ns – pipeline proven, sensor link not yet |

### Target (fix roadmap)

> Flowchart `diagrams/camera_target.mmd` (camera -> MC20901 -> PMOD -> Ganga)

![Camera target – MC20901 bridge to PMOD](diagrams/camera_target.png)

*Pictorial keeps the same nets as flowchart: `IMX219/OV13850` D-PHY ->
`MC20901` HS receiver -> `PMOD JA/JB` pairs -> `CSI-2 RX` -> Ganga
`V4L2 + DMA`; Ganga `I2C` drives sensor SCCB, Ganga `GPIO` drives
reset/power-down.*

| Action | Owner | Status 2026-09-15 |
|---|---|---|
| Add MIPI pad bundle (PMOD JA) to top + XDC | RTL (confidential) | **Done** – `cam_lp_p/n` JA1/JA2, `cam_hs_p/n` JA3/JA4, `cam_clk` JA7, `cam_sel` JA8 |
| Route sensor stimulus switch (pattern vs pads) | RTL (confidential) | **Done** – 2FF-synced mux, `cam_sel` 0 = pattern default |
| Re-run build, update `docs/BITSTREAM_GANGA.md` | Build | **Done** – `ganga_mipi_top_revB.bit` 3.65 MB, XSIM PASS both modes, DRC 0 err, WNS +4.945 ns |

## 2. Q2 – MIPI RX Subsystem IP in board files

> Flowchart `diagrams/bd_skeleton.mmd` (skeleton vs verified bitstream)

![BD skeleton vs verified bitstream](diagrams/bd_skeleton.png)

*Pictorial keeps the same nets as flowchart: `mipi_csi2_rx_subsystem:6.0`
+ `axi_interconnect` live only in `vivado/bd.tcl` (validate deferred);
the shipped bitstream comes from `vivado/run_bitstream.tcl` over soft
RTL only – no Xilinx MIPI IP in the netlist (report cells: BUFG/CARRY4/
LUT/FDCE only).*

### Evidence

| Fact | Trace |
|---|---|
| IP instantiated in skeleton only | `vivado/bd.tcl:29` (`xilinx.com:ip:mipi_csi2_rx_subsystem:6.0`), `bd.tcl:23` (`axi_interconnect`) |
| Skeleton never completed | `bd.tcl`: `validate_bd_design` commented out; no `.bd` artifact ships |
| Bitstream flow uses no IP catalog | `vivado/run_bitstream.tcl:21-38`: `add_files` (4 soft-RTL files + top) -> synth -> impl -> `write_bitstream`; no `create_bd_cell` |
| Roadmap | Full-custom path (`vivado/build_custom.tcl` + `vivado/bd.tcl`) wires the hard IP once SoC clocks exist |

## 3. Q3 – Where are the Shakti files?

> Flowchart `diagrams/shakti_files.mmd` (in-release vs external vs missing)

![Shakti file map](diagrams/shakti_files.png)

*Pictorial keeps the same nets as flowchart: this release carries docs +
Ganga-addressed driver + Ganga-compatible constraints + prebuilt
bitstream; the Shakti GC2025 project (external) carries BSV sources,
prebuilt Ganga/Yamuna `.bit/.mcs`, and GCSDK; missing here are
`mkSoc` Verilog (needs BSC build), SoC checkpoint for BD import, and
the full SoC+camera custom bitstream – all roadmap.*

### Evidence

| Fact | Trace |
|---|---|
| No BSV / mkSoc / fpga_top ships | Repo tree: `docs/`, `sw/`, `examples/`, `vivado/` only (see README) |
| Ganga enters three ways | (a) prebuilt `ganga_mipi_top.bit` release asset; (b) `sw/linux_v4l2/shakti_mipi.dts` Ganga map (`0x44Ax_xxxx`, PLIC IRQ); (c) `top_ganga_mipi.xdc` Arty pins matching Ganga board pins |
| SoC rebuild path documented | `vivado/build_custom.tcl` + `vivado/README.md` (`SHAKTI_GC2025`, BSC) |

## 4. Documents in this folder (V2X-style index)

| File | For | Contents |
|---|---|---|
| `07_Camera_Connectivity_QA.md` | **Systems/HW** | This file – Q1/Q2/Q3 verdicts + evidence |
| `DIAGRAMS.md` | **HW needing pictures** | Rendered PNGs + `.mmd` sources |
| `diagrams/camera_actual.mmd/.png` | **HW** | v0.3 actual: no camera pads |
| `diagrams/camera_target.mmd/.png` | **HW/layout** | Target: MC20901 -> PMOD |
| `diagrams/bd_skeleton.mmd/.png` | **FPGA** | Skeleton IP vs shipped netlist |
| `diagrams/shakti_files.mmd/.png` | **Systems** | Shakti file map |

## 5. How to regenerate diagrams

```bash
# From docs/diagrams/
npx -y @mermaid-js/mermaid-cli -i camera_actual.mmd -o camera_actual.png -b white
npx -y @mermaid-js/mermaid-cli -i camera_target.mmd -o camera_target.png -b white
npx -y @mermaid-js/mermaid-cli -i bd_skeleton.mmd -o bd_skeleton.png -b white
npx -y @mermaid-js/mermaid-cli -i shakti_files.mmd -o shakti_files.png -b white
```

> `.mmd` kept alongside `.png` – edit the `.mmd`, re-run `npx`, PNG updates.
> GitHub also renders the same sources live from fenced `mermaid` blocks.
