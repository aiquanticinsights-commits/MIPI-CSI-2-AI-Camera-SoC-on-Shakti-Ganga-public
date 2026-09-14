/*
 * MIPI CSI-2 AI Camera IP - Software Header
 *
 * Copyright (C) 2026 Open Source MIPI CSI-2 IP Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MIPI_CSI2_H
#define _MIPI_CSI2_H

#define CSI2_RX_REG_CONFIG        0x000
#define CSI2_RX_REG_STATUS        0x004
#define CSI2_RX_REG_FRAME_COUNT   0x008
#define CSI2_RX_REG_ERROR_COUNT   0x00C
#define CSI2_RX_REG_VC_MAP        0x010
#define CSI2_RX_REG_LANE_CONFIG   0x014
#define CSI2_RX_REG_TIMING        0x018

#define ISP_REG_FORMAT            0x000
#define ISP_REG_WIDTH             0x004
#define ISP_REG_HEIGHT            0x008
#define ISP_REG_AWB_GAIN_R        0x010
#define ISP_REG_AWB_GAIN_G        0x014
#define ISP_REG_AWB_GAIN_B        0x018
#define ISP_REG_AE_TARGET         0x020
#define ISP_REG_ANR_STRENGTH      0x024
#define ISP_REG_GAMMA_LUT         0x040
#define ISP_REG_CCM_MATRIX        0x080

#define AI_PREPROC_REG_CONFIG     0x000
#define AI_PREPROC_REG_INPUT_W    0x004
#define AI_PREPROC_REG_INPUT_H    0x008
#define AI_PREPROC_REG_OUTPUT_W   0x00C
#define AI_PREPROC_REG_OUTPUT_H   0x010
#define AI_PREPROC_REG_QUANT_CFG  0x014
#define AI_PREPROC_REG_FORMAT     0x018

#define CSI2_RX_IRQ_FRAME_DONE    0x01
#define CSI2_RX_IRQ_ERROR         0x02
#define CSI2_RX_IRQ_LINE_DONE     0x04

#define ISP_IRQ_AWB_DONE          0x01
#define ISP_IRQ_AE_DONE           0x02
#define ISP_IRQ_ISP_DONE          0x04

#define AI_PREPROC_IRQ_RESIZE_DONE 0x01
#define AI_PREPROC_IRQ_QUANT_DONE  0x02

#define SENSOR_OV13850    0x01
#define SENSOR_IMX219     0x02
#define SENSOR_OX08B40    0x03
#define SENSOR_IMX735     0x04
#define SENSOR_IMX324     0x05

#define DMA_CH0_SENSOR_TO_ISP   0x01
#define DMA_CH1_ISP_TO_AI       0x02
#define DMA_CH2_AI_TO_OUTPUT    0x03

#define GPIO_LED0       0
#define GPIO_LED1       1
#define GPIO_SENSOR_RST 2
#define GPIO_SENSOR_PWD 3
#define GPIO_UART_TX    4
#define GPIO_UART_RX    5

#define UART_BASE       0x40000000
#define I2C_BASE        0x40001000
#define DMA_BASE        0x40002000
#define CSI2_RX_BASE    0x40003000
#define ISP_BASE        0x40004000
#define AI_PREPROC_BASE 0x40005000
#define TIMER_BASE      0x40006000
#define GPIO_BASE       0x40007000

#define SYS_CLOCK_FREQ  25000000
#define TARGET_FPS      30
#define BUFFER_COUNT    4

struct sensor_config {
    u16 width;
    u16 height;
    u8  bayer_pattern;
    u8  lane_count;
    u32 link_speed;
    u32 frame_rate;
    u32 exposure;
    u32 gain;
    u32 white_balance;
};

struct frame_buffer {
    void *virt;
    u32  phys;
    u32  size;
    u32  width;
    u32  height;
    u32  format;
    u32  filled;
};

int mipi_csi2_init(void);
int mipi_csi2_configure_sensor(u8 sensor_id, struct sensor_config *cfg);
int mipi_csi2_start_stream(void);
int mipi_csi2_stop_stream(void);
int mipi_csi2_wait_frame(void);
int mipi_csi2_get_frame(void);
int mipi_csi2_irq_handler(void);
int mipi_csi2_dma_init(void);
int mipi_csi2_dma_transfer(u32 ch, u32 src, u32 dst, u32 size);
int mipi_csi2_isp_configure(u32 width, u32 height, u32 format);
int mipi_csi2_ai_preproc_configure(u32 in_w, u32 in_h, u32 out_w, u32 out_h);
int mipi_csi2_uart_putc(char c);
int mipi_csi2_uart_puts(const char *s);
int mipi_csi2_i2c_write(u16 addr, u8 *data, u16 len);
int mipi_csi2_i2c_read(u16 addr, u8 *data, u16 len);
void mipi_csi2_delay_ms(u32 ms);
void mipi_csi2_delay_us(u32 us);

#endif /* _MIPI_CSI2_H */