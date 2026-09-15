# Known Issues (Public)

Issues affecting users of the public release (prebuilt bitstream +
V4L2 driver). Internal implementation errata are not published.

## K-PUB-001 – First frame after programming may be partial

**Symptom:** The first captured frame after FPGA programming can show
tearing. **Workaround:** discard the first frame; capture from the
second frame on. **Status:** documented limitation.

## K-PUB-002 – Sensor needs power-cycle on mode change

**Symptom:** Changing resolution at runtime without a sensor reset can
stall the stream. **Workaround:** power-cycle the camera (GPIO) before
`s_stream(1)` after a mode change. **Status:** documented limitation.

## K-PUB-003 – `led[3]` (error LED) lit after hot-plug

**Symptom:** Unplugging/replugging the camera while streaming lights
the error LED. **Workaround:** reprogram the bitstream or toggle the
sensor reset GPIO, then restart streaming. **Status:** documented
limitation.

## K-PUB-004 – Rev-B boots in pattern mode (camera silent until selected)

**Symptom:** With the rev-B bitstream, no camera traffic flows after
programming even with the module wired. **Cause:** `cam_sel` (JA8)
defaults low = internal pattern. **Workaround:** drive JA8 high for
camera input. **Status:** documented behavior.

## Reporting new issues

Open a GitHub issue with: board revision, camera module, bitstream
version (`v0.3-ganga-public` or `v0.3-ganga-revB`), kernel log
(`dmesg`), and steps to reproduce.
