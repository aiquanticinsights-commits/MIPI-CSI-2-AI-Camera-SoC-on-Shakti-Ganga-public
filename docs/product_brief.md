# Product Brief – MIPI CSI-2 AI Camera SoC on Shakti Ganga

**Release:** v0.3-ganga-public | **Target:** Arty A7-100T (xc7a100tcsg324-1)

## Summary

MIPI CSI-2 camera pipeline (hard D-PHY via MC20901 bridge) with
multi-sensor aggregation, ISP (Bayer to RGB), and AI preprocessing
(resize + INT8 quantization), controlled by the Shakti Ganga RV64
application core running Linux with a V4L2 subdevice driver.

## Target applications

- Automotive: surround-view, driver monitoring, ADAS sensor fusion
- Robotics: multi-camera navigation, object detection
- Industrial: machine vision, quality inspection
- Edge AI: camera + preprocessing for YOLO/MobileNet-class inference

## Pipeline

```
Sensor -> MC20901 (D-PHY, 1-4 lanes) -> CSI-2 RX -> VC merge
  -> ISP (demosaic, AWB/AE/ANR, CCM, gamma) -> AI preproc
  (resize, INT8 quant) -> AXI DMA -> DDR -> V4L2
```

- CSI-2 RX: 1-4 lanes, RAW6-RAW24, YUV422, RGB, virtual channels
- Aggregation: virtual-channel merge for multi-sensor input
- ISP: 10-bit Bayer to 24-bit RGB
- AI preproc: downscale (e.g. 3840x2160 to 384x640 class) + INT8
- Control: Shakti Ganga RV64IMAC (Linux), PLIC interrupts, I2C sensor config

## What you get in this public release

- Prebuilt bitstream (`ganga_mipi_top.bit`, 3.7 MB) via Releases
- Linux V4L2 driver + device-tree bindings (`sw/linux_v4l2/`)
- Userspace capture example (`sw/examples/`)
- Register map, setup and verification docs (`docs/`)

The implementation RTL is available under commercial license;
contact via a GitHub issue titled "Commercial licensing".
