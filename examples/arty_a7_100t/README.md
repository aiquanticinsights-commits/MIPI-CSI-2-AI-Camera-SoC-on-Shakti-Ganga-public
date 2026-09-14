# Arty A7-100T – Programming Guide (Public)

## Files

- `top_ganga_mipi.xdc` – pin constraints for the prebuilt bitstream
  (100 MHz `sys_clk` E3, `sys_rst` C2, LEDs, UART). Board pins follow
  the public Digilent Arty A7 reference manual.
- Bitstream: download `ganga_mipi_top.bit` (3.7 MB) from
  [Releases](../../../releases) (v0.3-ganga-public).

## Program via openocd

```bash
sudo apt install -y openocd
openocd -f interface/ftdi/digilent-hs1.cfg \
  -c "init; pld load 0 ganga_mipi_top.bit; exit"
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
