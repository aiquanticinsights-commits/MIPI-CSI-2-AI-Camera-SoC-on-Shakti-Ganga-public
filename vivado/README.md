# Vivado Build Scripts (Public)

Reproduce the verified `ganga_mipi_top.bit` (see `docs/BITSTREAM_GANGA.md`).

## Files

| Script | Purpose |
|--------|---------|
| `run_sim.tcl` | XSIM behavioral simulation (20 us, self-checking) |
| `run_bitstream.tcl` | Synthesis + implementation + bitstream |
| `bd.tcl` | Block-design skeleton (MIPI RX IP + AXI interconnect) |
| `build_custom.tcl` | Full custom path with Shakti SoC rebuild |

## Prerequisites

- Vivado 2026.1+ with Artix-7 support
- Digilent `arty-a7-100` board files (override search path with
  `BOARD_REPO` env var if installed elsewhere)
- Xilinx MIPI CSI-2 RX Subsystem 6.0 + AXI Interconnect (in IP catalog)
- Implementation RTL tree (commercial license) providing `rtl/` with
  the block layout in `docs/architecture_overview.md`, or set
  `RTL_ROOT` env var to its location
- For the SoC rebuild path: BSC + a Shakti GC2025 checkout
  (`SHAKTI_GC2025` env var)

## Run (from repo root)

```powershell
vivado -mode batch -source vivado/run_sim.tcl
vivado -mode batch -source vivado/run_bitstream.tcl
```

Without the RTL tree the scripts exit with a message pointing at the
prebuilt bitstream (`examples/arty_a7_100t/ganga_mipi_top.bit`) and
the commercial-licensing contact in `README.md`.
