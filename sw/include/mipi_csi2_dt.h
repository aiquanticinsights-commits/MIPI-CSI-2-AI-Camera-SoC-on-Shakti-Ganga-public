/*
 * Device Tree Bindings for MIPI CSI-2 AI Camera IP
 *
 * Copyright (C) 2026 Open Source MIPI CSI-2 IP Project
 * SPDX-License-Identifier: Apache-2.0
 *
 * Required properties:
 * - compatible: "open-source,mipi-csi2-ai-camera"
 * - reg: Base address and length of MMIO registers
 * - interrupts: Interrupt specifier
 * - clocks: Phandle to reference clock
 * - reset-gpios: GPIO for hardware reset
 * - power-gpios: GPIO for power enable
 * - avdd-supply: Regulator for analog supply
 * - dvdd-supply: Regulator for digital supply
 *
 * Optional properties:
 * - num-lanes: Number of MIPI CSI-2 lanes (default: 4)
 * - link-speed: Link speed in Mbps (default: 1000)
 * - sensor-type: Sensor type string (ov13850, imx219, etc.)
 * - status: "okay" or "disabled" (default: "okay")
 */

/* Example device tree node:
 *
 * mipi_csi2: camera@40003000 {
 *     compatible = "open-source,mipi-csi2-ai-camera";
 *     reg = <0x40003000 0x1000>;
 *     interrupts = <0 42 4>;
 *     clocks = <&clk_mipi 25000000>;
 *     reset-gpios = <&gpio 2 GPIO_ACTIVE_HIGH>;
 *     power-gpios = <&gpio 3 GPIO_ACTIVE_HIGH>;
 *     avdd-supply = <&avdd_reg>;
 *     dvdd-supply = <&dvdd_reg>;
 *     num-lanes = <4>;
 *     link-speed = <1000>;
 *     status = "okay";
 * };
 */

#define MIPI_CSI2_DT_COMPATIBLE "open-source,mipi-csi2-ai-camera"
#define MIPI_CSI2_REG_BASE    0x40003000
#define MIPI_CSI2_REG_SIZE    0x1000
#define MIPI_CSI2_IRQ         42
#define MIPI_CSI2_CLK_FREQ    25000000