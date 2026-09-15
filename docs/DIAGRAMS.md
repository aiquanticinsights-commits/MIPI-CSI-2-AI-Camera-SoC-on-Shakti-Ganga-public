# Diagrams (Pictorial + Flowchart)

> **For the HW engineer who needs a picture, not just `mermaid` code.**
> All diagrams below are **rendered PNGs** from the `mermaid` flowcharts
> in `docs/diagrams/`, plus the flowchart source kept for VS Code preview.
> Open this file in VS Code or GitHub to see both.

## 1. Camera actual – v0.3 pattern-driven (07_QA Q1)

> Flowchart `diagrams/camera_actual.mmd` (pattern source -> pipeline -> Arty pins)

![Camera actual](diagrams/camera_actual.png)

*Pictorial keeps the same nets as flowchart: `tick_q/sot_phase` ->
`csi2_rx` -> `vc_merge` -> `isp_pipeline` -> `ai_preproc`;
pins `E3 sys_clk`, `C2 sys_rst`, `led[0..3]`, `D10/A9 uart` – no MIPI.*

## 2. Camera target – MC20901 bridge (07_QA Q1 roadmap)

> Flowchart `diagrams/camera_target.mmd` (camera -> bridge -> PMOD -> Ganga)

![Camera target](diagrams/camera_target.png)

*Pictorial stack: `IMX219/OV13850` -> `MC20901` -> `PMOD JA/JB` ->
`CSI-2 RX` -> Ganga `V4L2 + DMA`; Ganga `I2C` = SCCB, `GPIO` = RST/PWDN.*

## 3. BD skeleton vs verified bitstream (07_QA Q2)

> Flowchart `diagrams/bd_skeleton.mmd` (skeleton IP vs shipped netlist)

![BD skeleton](diagrams/bd_skeleton.png)

*Pictorial keeps the same nets as flowchart: `mipi_csi2_rx_subsystem:6.0`
lives in `vivado/bd.tcl` only; shipped `.bit` is soft RTL via
`vivado/run_bitstream.tcl`.*

## 4. Shakti file map (07_QA Q3)

> Flowchart `diagrams/shakti_files.mmd` (in-release vs external vs missing)

![Shakti files](diagrams/shakti_files.png)

*Pictorial keeps the same nets as flowchart: release = docs + driver +
constraints + prebuilt bit; GC2025 project = BSV + `.bit/.mcs` + GCSDK;
missing = `mkSoc` Verilog + SoC checkpoint + full custom bitstream.*

## 5. How to regenerate (for your board spin)

```bash
# From docs/diagrams/
npx -y @mermaid-js/mermaid-cli -i camera_actual.mmd -o camera_actual.png -b white
npx -y @mermaid-js/mermaid-cli -i camera_target.mmd -o camera_target.png -b white
npx -y @mermaid-js/mermaid-cli -i bd_skeleton.mmd -o bd_skeleton.png -b white
npx -y @mermaid-js/mermaid-cli -i shakti_files.mmd -o shakti_files.png -b white
```

> All `.mmd` kept alongside `.png` in `docs/diagrams/` – edit the `.mmd`,
> re-run `npx`, PNG updates. VS Code renders `mermaid` live without PNG.

## 6. What to give the layout engineer (one ZIP)

- This folder `docs/` (4 PNG + 4 MMD + `DIAGRAMS.md` + `07_Camera_Connectivity_QA.md`)
- Camera module part number + PMOD cable pinout (target diagram §2)
- Arty A7-100T reference manual (E3/C2/LED/UART pins already in `examples/`)
