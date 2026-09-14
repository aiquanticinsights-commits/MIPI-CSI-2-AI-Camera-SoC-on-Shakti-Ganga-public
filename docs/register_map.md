# Register Map

Peripheral base addresses (from `sw/include/mipi_csi2.h`). All registers
are 32-bit; the map targets the SoC control integration and the V4L2
driver.

## Memory Map

| Base | Peripheral | Notes |
|---|---|---|
| 0x44A0_0000 | CSI2_RX | MIPI CSI-2 RX (hard via MC20901) |
| 0x44A1_0000 | VC_MERGE | aggregator |
| 0x44A2_0000 | ISP | pipeline |
| 0x44B0_0000 | DMA | AXI DMA to DDR |
| 0x44C0_0000 | I2C | sensor configuration |
| 0x44D0_0000 | GPIO | LEDs, sensor reset/power |
| 0x0001_0000 | PLIC/CLINT | Shakti SoC interrupt/timer |

## CSI-2 RX (`CSI2_RX_BASE`)

| Offset | Register | Description |
|---|---|---|
| 0x000 | `CSI2_RX_REG_CONFIG` | link configuration |
| 0x004 | `CSI2_RX_REG_STATUS` | status (frame done / error) |
| 0x008 | `CSI2_RX_REG_FRAME_COUNT` | frame counter |
| 0x00C | `CSI2_RX_REG_ERROR_COUNT` | error counter |
| 0x010 | `CSI2_RX_REG_VC_MAP` | virtual channel map |
| 0x014 | `CSI2_RX_REG_LANE_CONFIG` | lane configuration |
| 0x018 | `CSI2_RX_REG_TIMING` | timing registers |

**Interrupts** (`CSI2_RX_IRQ_*`): `FRAME_DONE` (0x01), `ERROR` (0x02),
`LINE_DONE` (0x04).

## ISP Pipeline (`ISP_BASE`)

| Offset | Register | Description |
|---|---|---|
| 0x000 | `ISP_REG_FORMAT` | output format |
| 0x004 | `ISP_REG_WIDTH` | frame width |
| 0x008 | `ISP_REG_HEIGHT` | frame height |
| 0x010 | `ISP_REG_AWB_GAIN_R` | AWB red gain |
| 0x014 | `ISP_REG_AWB_GAIN_G` | AWB green gain |
| 0x018 | `ISP_REG_AWB_GAIN_B` | AWB blue gain |
| 0x020 | `ISP_REG_AE_TARGET` | auto-exposure target |
| 0x024 | `ISP_REG_ANR_STRENGTH` | noise-reduction strength |
| 0x040 | `ISP_REG_GAMMA_LUT` | gamma LUT (block-addressable) |
| 0x080 | `ISP_REG_CCM_MATRIX` | color-correction matrix |

**Interrupts:** `AWB_DONE` (0x01), `AE_DONE` (0x02), `ISP_DONE` (0x04).

## AI Preprocessing (`AI_PREPROC_BASE`)

| Offset | Register | Description |
|---|---|---|
| 0x000 | `AI_PREPROC_REG_CONFIG` | configuration |
| 0x004 | `AI_PREPROC_REG_INPUT_W` | input width |
| 0x008 | `AI_PREPROC_REG_INPUT_H` | input height |
| 0x00C | `AI_PREPROC_REG_OUTPUT_W` | output width |
| 0x010 | `AI_PREPROC_REG_OUTPUT_H` | output height |
| 0x014 | `AI_PREPROC_REG_QUANT_CFG` | INT8 quantization config |
| 0x018 | `AI_PREPROC_REG_FORMAT` | output format |

**Interrupts:** `RESIZE_DONE` (0x01), `QUANT_DONE` (0x02).

## DMA Channels

| Channel | Path | Constant |
|---|---|---|
| 0 | sensor → ISP | `DMA_CH0_SENSOR_TO_ISP` |
| 1 | ISP → AI | `DMA_CH1_ISP_TO_AI` |
| 2 | AI → output | `DMA_CH2_AI_TO_OUTPUT` |

## GPIO

| Bit | Function | Constant |
|---|---|---|
| 0 | LED0 | `GPIO_LED0` |
| 1 | LED1 | `GPIO_LED1` |
| 2 | sensor reset | `GPIO_SENSOR_RST` |
| 3 | sensor power-down | `GPIO_SENSOR_PWD` |
| 4 | UART TX | `GPIO_UART_TX` |
| 5 | UART RX | `GPIO_UART_RX` |

## System

- `SYS_CLOCK_FREQ` 25 MHz
- `TARGET_FPS` 30
- `BUFFER_COUNT` 4 (frame buffers)

> **Note:** the register map is defined by the software header
> (`sw/include/mipi_csi2.h`). RTL blocks expose behaviour via their ports;
> the SoC register attachment is part of Phase 6 integration.

