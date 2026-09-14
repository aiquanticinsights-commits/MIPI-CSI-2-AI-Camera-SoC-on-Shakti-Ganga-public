# Setup Guide – Host Tools and Board

## 1. Host (Ubuntu 22.04 / 24.04)

```bash
sudo apt update && sudo apt install -y \
  openocd gcc make python3 dtc minicom
```

- `openocd` 0.12+: programs the Arty via USB-JTAG.
- `gcc`: builds the userspace capture example (`sw/examples/`).
- `dtc`: compiles the device-tree overlay (`sw/linux_v4l2/shakti_mipi.dts`).
- `minicom`: UART console (115200 8N1) for Shakti Ganga Linux.

Optional (only to rebuild the Vivado project, not needed for evaluation):

- Vivado 2026.1+ with Artix-7 support + Digilent board files
  (`vivado-boards` repo, `arty-a7-100` part files).

## 2. Arty A7-100T board

1. Power via USB (J10). Two serial ports appear: JTAG + UART.
2. Connect the camera module through the MC20901 D-PHY bridge to
   PMOD JA/JB (see `examples/arty_a7_100t/top_ganga_mipi.xdc` for pins).
3. Sensor I2C (SCCB) and reset/power-down are driven by the Shakti
   SoC GPIOs – no host-side wiring needed beyond the module cable.

## 3. Shakti Ganga Linux

- SoC: Shakti C-Class RV64IMAC, MMU, PLIC/CLINT, 256 MB DDR
  (see Shakti project GC2025 pages for the SoC itself).
- Boot the provided Ganga Linux image on the Arty, then follow
  `docs/quickstart.md` steps 3-4 (driver + capture).
- UART console: `minicom -D /dev/ttyUSB1 -b 115200`.

## 4. Verifying the setup

- After programming, `led[2]` blinks and `led[3]` stays off.
- `dmesg` shows the `shakti-mipi-csi2` probe message.
- `./v4l2_capture` exits 0 and writes a non-empty `frame.raw`.
