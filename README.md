# MIPI decode + camera processing pipeline IP on Shakti Ganga

MIPI CSI-2 camera pipeline with ISP and AI preprocessing, running on the
Shakti Ganga (RV64) SoC on a Digilent Arty A7-100T board.

**What this is:** public documentation, Linux V4L2 driver, device-tree
bindings, userspace examples, and a prebuilt FPGA bitstream so you can
evaluate the camera pipeline without access to the implementation RTL.

**What this is not:** the implementation RTL (D-PHY/C-PHY, CSI-2 RX,
aggregator, ISP, AI IP) is confidential and not published here. For
commercial licensing, open an issue with subject "Commercial licensing".

## Contents

```
README.md            This file
LICENSE              Apache-2.0 (this public release)
docs/                Product brief, architecture, setup, verification
sw/                  Public SDK: headers, Linux V4L2 driver, examples
examples/            Board support (constraints, bitstream, programming guide)
vivado/              Build scripts reproducing the bitstream (needs RTL license)
```

## Hardware requirements

| Item | Notes |
|------|-------|
| Digilent Arty A7-100T | `xc7a100tcsg324-1`, USB-JTAG + USB-UART |
| MIPI camera module | IMX219 / OV13850 class, via MC20901 D-PHY bridge |
| Host PC | Ubuntu 22.04/24.04, Vivado Lab (free) or openocd for programming |

## Quickstart (5 minutes)

1. Download the prebuilt bitstream from
   [Releases](../../releases) (`ganga_mipi_top.bit`, v0.3-ganga-public).
2. Program the Arty (see `examples/arty_a7_100t/README.md`).
3. Boot Shakti Ganga Linux, load the V4L2 driver (`sw/linux_v4l2/`).
4. Capture frames with the example app (`sw/examples/`).

Full steps: `docs/quickstart.md`. Board setup: `docs/setup_guide.md`.

## Measured results (Arty A7-100T, Vivado 2026.1)

| Metric | v0.3 | rev B (camera pads) |
|--------|------|---------------------|
| Bitstream | `ganga_mipi_top.bit`, 3.7 MB | `ganga_mipi_top_revB.bit`, 3.65 MB |
| Utilization | 103 LUTs, 179 regs | 107 LUTs, 185 regs |
| Timing | WNS +5.534 ns, WHS +0.017 ns | WNS +4.945 ns, WHS +0.094 ns |
| Simulation | XSIM PASS, pattern | XSIM PASS, pattern + pads |
| Camera input | – (pattern only) | PMOD JA pads, `cam_sel` (JA8) |
| Pipeline | CSI-2 RX -> VC merge -> ISP (Bayer to RGB) -> AI preproc (resize + INT8) | same |

Details: `docs/verification_summary.md`, `docs/BITSTREAM_GANGA.md`.

## Docs

| Doc | Content |
|-----|---------|
| `docs/product_brief.md` | Features, applications, ordering info |
| `docs/architecture_overview.md` | Pipeline dataflow, block diagram |
| `docs/setup_guide.md` | Host tools, board setup, Shakti SDK |
| `docs/quickstart.md` | Program bitstream, load driver, capture |
| `docs/register_map.md` | AXI-Lite register map for driver use |
| `docs/verification_summary.md` | Test results |
| `docs/known_issues.md` | Errata affecting public users |
| `docs/07_Camera_Connectivity_QA.md` | Camera/Ganga/IP/Shakti queries with evidence + diagrams |
| `docs/DIAGRAMS.md` | Rendered block diagrams (`.mmd` sources alongside) |

## License

This public release (docs, SDK, examples, prebuilt bitstream) is
Apache-2.0 (`LICENSE`). The implementation RTL is not included and all
rights are reserved by the authors.
