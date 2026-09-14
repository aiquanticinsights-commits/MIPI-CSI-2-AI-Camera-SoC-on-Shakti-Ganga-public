# Quickstart – MIPI CSI-2 AI Camera on Shakti Ganga

Goal: program the prebuilt bitstream, load the V4L2 driver, capture a frame.

## 1. Prerequisites

- Arty A7-100T connected over USB (JTAG + UART). Note the serial ports:
  `ls /dev/ttyUSB*` (typically `ttyUSB0` = JTAG, `ttyUSB1` = UART).
- Ubuntu 22.04/24.04 host with `openocd` (or Vivado Lab, free download):
  `sudo apt install -y openocd`
- Prebuilt bitstream `examples/arty_a7_100t/ganga_mipi_top.bit`
  (v0.3-ganga-public, 3.7 MB; also attached to the GitHub Release).

## 2. Program the FPGA

```bash
# Via openocd (Shakti JTAG config, 100 kHz init)
openocd -f interface/ftdi/digilent-hs1.cfg \
  -c "init; pld load 0 ganga_mipi_top.bit; exit"
```

LEDs on success: `led[0]` toggles per frame, `led[2]` blinks ~12 Hz,
`led[3]` off (on = CSI error). Details: `examples/arty_a7_100t/README.md`.

## 3. Boot Ganga Linux and load the driver

```bash
# On the Shakti Ganga Linux console (UART 115200)
# Copy sw/linux_v4l2/shakti_mipi.c and sw/linux_v4l2/shakti_mipi.dts
# into your kernel tree, enable CONFIG_VIDEO_SHAKTI_MIPI, rebuild dtb:
dtc -I dts -O dtb -o shakti_mipi.dtb sw/linux_v4l2/shakti_mipi.dts
# Boot with the dtb, then:
modprobe shakti-mipi-csi2
dmesg | tail   # expect: shakti-mipi-csi2 44a00000.mipi: probed
```

Driver source and bindings: `sw/linux_v4l2/README.md`.

## 4. Capture a frame

```bash
gcc -o v4l2_capture sw/examples/v4l2_capture.c
./v4l2_capture /dev/video0 640 480 frame.raw
# Convert/inspect frame.raw (RAW8, 640x480) with your preferred tool
```

Example source: `sw/examples/v4l2_capture.c`.

## 5. Next steps

- Register-level control: `docs/register_map.md`
- Board details and constraints: `examples/arty_a7_100t/`
- Issues: `docs/known_issues.md`
