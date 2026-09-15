# Query Responses – Shakti Ganga MIPI Port (v0.3-ganga-public)

Short GitHub-ready answers to the three received queries. Each answer points
to its block diagram (rendered below and in `docs/DIAGRAMS.md`; sources in
`docs/diagrams/*.mmd`). Full evidence tables: `docs/07_Camera_Connectivity_QA.md`.

---

## Q1 – How is the camera connected to Ganga? (No MIPI signals in constraints)

**Answer: it is not – yet. Your observation is correct, and it is by design
of this release.** The v0.3 bitstream proves the pipeline with an internal
pattern source; no camera pads exist. The top exposes only `sys_clk`,
`sys_rst`, `led[0..3]`, `uart_txd/rxd`, and the XDC constrains exactly those
9 board pins – zero MIPI lane pins.

Actual connectivity (v0.3, what shipped):

![Camera actual – pattern-driven, no camera pads](diagrams/camera_actual.png)

*Source: `docs/diagrams/camera_actual.mmd`. Detail: `docs/07_Camera_Connectivity_QA.md` §1.*

Target connectivity (rev B roadmap – camera via MC20901 bridge to PMOD,
Ganga I2C/GPIO controlling the sensor):

![Camera target – MC20901 bridge to PMOD](diagrams/camera_target.png)

*Source: `docs/diagrams/camera_target.mmd`.*

**Actions:** add MIPI pad bundle (PMOD JA/JB) to top + XDC, add
pattern-vs-pads select register, rebuild via `vivado/run_bitstream.tcl`,
publish rev-B bitstream.

---

## Q2 – MIPI_RX_SYSTEM IP seen in the board files

**Answer: the Xilinx `mipi_csi2_rx_subsystem:6.0` IP exists only in the
unfinished block-design skeleton – it is NOT in the shipped bitstream.**
`vivado/bd.tcl:29` instantiates the IP, but that BD was never completed
(`validate_bd_design` still commented out, no `.bd` artifact). The verified
bitstream was built by `vivado/run_bitstream.tcl` from soft RTL only –
the netlist proves it (BUFG/CARRY4/LUT/FDCE cells, no Xilinx IP).

Skeleton vs shipped (see the split):

![BD skeleton vs verified bitstream](diagrams/bd_skeleton.png)

*Source: `docs/diagrams/bd_skeleton.mmd`. Detail: `docs/07_Camera_Connectivity_QA.md` §2.*

**Actions:** none required for evaluation – use the prebuilt bitstream.
The hard IP enters in the full-custom path (`vivado/build_custom.tcl` +
`vivado/bd.tcl`) once SoC clocks exist.

---

## Q3 – No Shakti files visible

**Answer: correct – no Shakti BSV/Verilog ships in this public release,
and Ganga still enters three documented ways.** Present here: Ganga-addressed
driver + device tree (`sw/linux_v4l2/`, `0x44Ax_xxxx`, PLIC IRQ),
Ganga-compatible Arty constraints, and the prebuilt bitstream (release
asset). The Shakti SoC sources live in the external Shakti GC2025 project;
`mkSoc` Verilog (needs BSC build), the SoC checkpoint for BD import, and
the full SoC+camera custom bitstream are roadmap items.

File map (in-release vs external vs missing):

![Shakti file map](diagrams/shakti_files.png)

*Source: `docs/diagrams/shakti_files.mmd`. Detail: `docs/07_Camera_Connectivity_QA.md` §3.*

**Actions:** evaluate with the prebuilt bitstream + V4L2 driver per
`docs/quickstart.md`; SoC rebuild path is documented in
`vivado/README.md` (`SHAKTI_GC2025`, BSC prerequisites).

---

*Reply template for the thread: Q1 valid-by-design (rev B planned), Q2/Q3
clarified – diagrams above, evidence in `docs/07_Camera_Connectivity_QA.md`.*
