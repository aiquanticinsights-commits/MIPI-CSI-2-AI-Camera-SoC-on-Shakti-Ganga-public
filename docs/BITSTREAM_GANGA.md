# Ganga MIPI Bitstream – Build and Verification Record

**Tool:** Vivado 2026.1 | **Part:** xc7a100tcsg324-1 (Arty A7-100T) |
**Releases:** v0.3-ganga-public, v0.3-ganga-revB

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

## Rev B (2026-09-15) – camera pads added

- Change: pad bundle on PMOD JA (`cam_lp_p/n` JA1/JA2, `cam_hs_p/n`
  JA3/JA4, `cam_clk` JA7, `cam_sel` JA8; LVCMOS33) + synchronized
  pattern/pads mux (`cam_sel` 0 = pattern default, 1 = pads).
- XSIM two-phase: pattern frames flowing, then pads frames flowing
  with mux selected (`PASS` both modes).
- Synthesis: 0 errors; ~110 LUTs, 180 FDCE + 5 FDPE.
- Implementation: routing 0 failed nets, DRC 0 errors.
- Timing: WNS +4.945 ns, WHS +0.094 ns, 0 failing – all constraints met.
- Bitstream: `ganga_mipi_top_revB.bit`, 3.65 MB, "Bitgen Completed
  Successfully".
- Utilization: Slice LUTs 107 (0.17%), Slice Registers 185 (0.15%).
- Cost of camera input: +4 LUTs / +6 registers over v0.3.

## Reproducing (licensed users with RTL access)

The bitstream is reproducible from the implementation sources; the
sources themselves are confidential and not part of this public
release. The programming procedure is documented in
`examples/arty_a7_100t/README.md`.

## Programming

See `docs/quickstart.md` step 2 and `examples/arty_a7_100t/README.md`.
LEDs: `led[0]` = frame heartbeat, `led[1]` = AI throughput,
`led[2]` = ~12 Hz blink, `led[3]` = CSI error (off = healthy).
