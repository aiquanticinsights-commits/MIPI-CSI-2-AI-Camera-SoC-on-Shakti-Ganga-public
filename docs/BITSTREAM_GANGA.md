# Ganga MIPI Bitstream – Build and Verification Record

**Tool:** Vivado 2026.1 | **Part:** xc7a100tcsg324-1 (Arty A7-100T) |
**Release:** v0.3-ganga-public

## Design under test

Pattern source -> CSI-2 RX (1 lane, 64x48) -> VC merge (1 sensor) ->
ISP (64x48) -> AI preproc (64x48 to 16x12). Ganga-compatible pinout:
100 MHz board clock, active-high reset button, 4 LEDs, UART TX/RX.
The same constraints shipped here (`examples/arty_a7_100t/`) were used.

## Evidence

- XSIM behavioral: frames flow end to end (`PASS`).
- Synthesis: 0 errors, 0 critical warnings (1 warning). Cells: 1 BUFG,
  36 CARRY4, ~100 LUTs, 174 FDCE + 5 FDPE.
- Implementation: routing completed, 0 failed nets, DRC 0 errors.
- Timing: WNS +5.534 ns, WHS +0.017 ns, 0 failing endpoints –
  all constraints met.
- Bitstream: `ganga_mipi_top.bit`, 3.7 MB, "Bitgen Completed Successfully".
- Utilization: Slice LUTs 103 (0.16%), Slice Registers 179 (0.14%).

## Reproducing (licensed users with RTL access)

The bitstream is reproducible from the implementation sources; the
sources themselves are confidential and not part of this public
release. The programming procedure is documented in
`examples/arty_a7_100t/README.md`.

## Programming

See `docs/quickstart.md` step 2 and `examples/arty_a7_100t/README.md`.
LEDs: `led[0]` = frame heartbeat, `led[1]` = AI throughput,
`led[2]` = ~12 Hz blink, `led[3]` = CSI error (off = healthy).
