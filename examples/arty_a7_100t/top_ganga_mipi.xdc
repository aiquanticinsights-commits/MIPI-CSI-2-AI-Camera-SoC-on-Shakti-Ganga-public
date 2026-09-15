# top_ganga_mipi.xdc – Arty A7-100T (xc7a100tcsg324-1), Ganga-compatible pinout
# Sys clock 100 MHz E3, reset C2, LEDs, UART (D10/A9). Matches Shakti Ganga board pins.
set_property -dict {PACKAGE_PIN E3 IOSTANDARD LVCMOS33} [get_ports sys_clk]
create_clock -period 10.000 -name sys_clk [get_ports sys_clk]
set_property -dict {PACKAGE_PIN C2 IOSTANDARD LVCMOS33} [get_ports sys_rst]
set_property -dict {PACKAGE_PIN H5 IOSTANDARD LVCMOS33} [get_ports {led[0]}]
set_property -dict {PACKAGE_PIN J5 IOSTANDARD LVCMOS33} [get_ports {led[1]}]
set_property -dict {PACKAGE_PIN T9 IOSTANDARD LVCMOS33} [get_ports {led[2]}]
set_property -dict {PACKAGE_PIN T10 IOSTANDARD LVCMOS33} [get_ports {led[3]}]
set_property -dict {PACKAGE_PIN D10 IOSTANDARD LVCMOS33} [get_ports uart_txd]
set_property -dict {PACKAGE_PIN A9 IOSTANDARD LVCMOS33} [get_ports uart_rxd]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets sys_rst_IBUF]
# Rev-B camera pads, PMOD JA, LVCMOS33 (MC20901 bridge single-ended model).
# Pins from the Digilent arty-a7-100 board files (E.0). Used by
# ganga_mipi_top_revB.bit; v0.3 ganga_mipi_top.bit ignores these pins.
set_property -dict {PACKAGE_PIN G13 IOSTANDARD LVCMOS33} [get_ports cam_lp_p]
set_property -dict {PACKAGE_PIN B11 IOSTANDARD LVCMOS33} [get_ports cam_lp_n]
set_property -dict {PACKAGE_PIN A11 IOSTANDARD LVCMOS33} [get_ports cam_hs_p]
set_property -dict {PACKAGE_PIN D12 IOSTANDARD LVCMOS33} [get_ports cam_hs_n]
set_property -dict {PACKAGE_PIN D13 IOSTANDARD LVCMOS33} [get_ports cam_clk]
set_property -dict {PACKAGE_PIN B18 IOSTANDARD LVCMOS33} [get_ports cam_sel]
