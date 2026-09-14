# Linux V4L2 Driver – Shakti MIPI CSI-2 (Public SDK)

V4L2 subdevice driver for the MIPI CSI-2 AI camera pipeline on
Shakti Ganga (RV64, MMU), plus device-tree bindings.

## Files

| File | Purpose |
|------|---------|
| `shakti_mipi.c` | V4L2 subdevice driver (`compatible = "shakti,mipi-csi2-ganga"`) |
| `shakti_mipi.dts` | Device-tree overlay (AXI-Lite regs, PLIC IRQ, sensor I2C) |
| `../include/mipi_csi2.h` | Register/IOCTL API header |
| `../include/mipi_csi2_dt.h` | Device-tree binding macros |

## Address map (see `docs/register_map.md`)

- `0x44A0_0000` MIPI CSI-2 RX, `0x44A1_0000` VC merge,
  `0x44A2_0000` ISP, `0x44A3_0000` AI preproc
- `0x44B0_0000` AXI DMA, `0x44C0_0000` I2C, `0x44D0_0000` GPIO

## Build against your Ganga kernel

```bash
# Copy into the kernel tree and enable the driver:
cp shakti_mipi.c <kernel>/drivers/media/i2c/
cp shakti_mipi.dts <kernel>/arch/riscv/boot/dts/shakti/
# enable CONFIG_VIDEO_SHAKTI_MIPI, rebuild kernel + dtb
dtc -I dts -O dtb -o shakti_mipi.dtb shakti_mipi.dts
modprobe shakti-mipi-csi2
dmesg | tail   # expect: probed at 44a00000
```

## Capture

See `../examples/v4l2_capture.c` and `docs/quickstart.md` step 4.
