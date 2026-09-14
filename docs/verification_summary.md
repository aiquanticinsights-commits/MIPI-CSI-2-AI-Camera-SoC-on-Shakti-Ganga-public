# Verification Summary (Public)

Measured on Arty A7-100T (`xc7a100tcsg324-1`), Vivado 2026.1.

## Results

| Check | Result |
|-------|--------|
| XSIM behavioral simulation | PASS – pixel frames flow end to end |
| Synthesis | 0 errors, 0 critical warnings |
| Implementation (route) | 0 failed nets, DRC 0 errors |
| Timing | WNS +5.534 ns, WHS +0.017 ns, 0 failing endpoints |
| Utilization | 103 LUTs (0.16%), 179 registers (0.14%) |
| Bitstream | `ganga_mipi_top.bit`, 3.7 MB, Bitgen completed successfully |
| Board smoke test | Frame LED toggles, error LED off, UART heartbeat alive |

Full report: `docs/BITSTREAM_GANGA.md`.

## Method (summary)

1. Behavioral simulation of the full pipeline with a sensor stimulus
   model (XSIM, 20 us, self-checking).
2. Synthesis + place-and-route with 100 MHz board-clock constraint.
3. Timing signoff (`report_timing_summary`), DRC, bitstream generation.
4. On-board smoke test: program flash, observe frame/error LEDs and
   capture one frame through the V4L2 driver.

Detailed internal testbenches, formal proofs, and coverage data are
part of the confidential implementation and are not published.
