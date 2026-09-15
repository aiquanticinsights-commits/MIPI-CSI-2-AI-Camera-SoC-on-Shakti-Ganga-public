# Arty A7-100T – Programming Guide (Public)

## Files

- `top_ganga_mipi.xdc` – pin constraints for the prebuilt bitstream
  (100 MHz `sys_clk` E3, `sys_rst` C2, LEDs, UART). Board pins follow
  the public Digilent Arty A7 reference manual.
- Bitstreams, both in this directory (also attached to GitHub Releases):
  - `ganga_mipi_top.bit` (3.7 MB, v0.3 – pattern source only)
  - `ganga_mipi_top_revB.bit` (3.7 MB, rev B – adds camera pads, see below)

## Camera wiring (rev-B bitstream only)

Connect the MC20901 bridge single-ended outputs to PMOD JA:

| Bridge signal | PMOD | Arty pin |
|---------------|------|----------|
| Lane LP+ | JA1 | G13 |
| Lane LP- | JA2 | B11 |
| Lane HS data | JA3 | A11 |
| Lane HS data complement | JA4 | D12 |
| Byte clock | JA7 | D13 |
| Source select | JA8 | B18 |

`cam_sel` (JA8): low = internal pattern (default, back-compatible);
high = camera pads. Program `ganga_mipi_top_revB.bit` for camera input:

## Program via openocd

```bash
sudo apt install -y openocd
openocd -f interface/ftdi/digilent-hs1.cfg \
  -c "init; pld load 0 ganga_mipi_top_revB.bit; exit"
```

## Program via Vivado Lab (free)

Hardware Manager -> Open Target -> Add Device (xc7a100t) ->
Program Device -> select `ganga_mipi_top.bit`.

## Expected behavior

| Signal | Meaning |
|--------|---------|
| `led[0]` | Toggles per captured frame |
| `led[1]` | AI preprocessing throughput activity |
| `led[2]` | ~12 Hz heartbeat blink |
| `led[3]` | Off = healthy; on = CSI error (see `docs/known_issues.md`) |

UART (115200 8N1) carries the Ganga Linux console once the SoC image
boots; the camera pipeline heartbeat is visible on the LEDs
independently of Linux.
