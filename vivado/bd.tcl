# bd.tcl – Vivado IPI block design skeleton: MIPI CSI-2 AI camera on Shakti Ganga (RV64).
# Usage: vivado -mode batch -source vivado/bd.tcl   (from repo root)
# Prereqs: Vivado 2026.1+, Xilinx MIPI CSI-2 RX Subsystem 6.0 + AXI
# Interconnect in the IP catalog, Digilent arty-a7-100 board files, and the
# Shakti GC2025 checkout for the SoC side (set SHAKTI_GC2025 env var).
# Reference: Shakti GC2025 IP_Integration_with_Shakti_SoC.pdf.
if {[info exists ::env(BOARD_REPO)]} {
  set_param board.repoPaths $::env(BOARD_REPO)
} else {
  set_param board.repoPaths [file join $::env(APPDATA) Xilinx Vivado 2026.1 xhub board_store xilinx_board_store]
}
create_project mipi_ganga ./mipi_ganga -part xc7a100tcsg324-1 -force
set_property board_part digilentinc.com:arty-a7-100:part0:1.1 [current_project]

set SCRIPT_DIR [file dirname [file normalize [info script]]]
set ROOT [file normalize "$SCRIPT_DIR/.."]

# Create BD
create_bd_design "shakti_ganga_mipi"
# Shakti Ganga SoC side: import from a GC2025 checkout if present.
# Set SHAKTI_GC2025 to the root of a Shakti GC2025 clone; expects
# $SHAKTI_GC2025/hw/shakti_ganga.tcl when provided by that project.
if {[info exists ::env(SHAKTI_GC2025)]} {
  set gc_tcl [file join $::env(SHAKTI_GC2025) "hw" "shakti_ganga.tcl"]
  if {[file exists $gc_tcl]} { source $gc_tcl }
}

# Xilinx MIPI CSI-2 RX Subsystem (hard D-PHY via MC20901 bridge)
create_bd_cell -type ip -vlnv xilinx.com:ip:mipi_csi2_rx_subsystem:6.0 mipi_csi2_rx
set_property -dict [list CONFIG.C_DPHY_LANES {4} CONFIG.C_EN_CSI_V2_0 {true}] [get_bd_cells mipi_csi2_rx]

# AXI Interconnect for the camera register slaves
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_ic
set_property -dict [list CONFIG.NUM_MI {4}] [get_bd_cells axi_ic]

# Camera RTL (commercial tree): set RTL_ROOT env var or default to <root>/rtl
if {[info exists ::env(RTL_ROOT)]} { set rtl_root $::env(RTL_ROOT) } else { set rtl_root "$ROOT/rtl" }
# Expected block layout: aggregator/vc_merge, isp/isp_pipeline,
# ai_preproc/ai_preproc, plus AXI wrappers (see docs/architecture_overview.md)
# add_files -norecurse [glob $rtl_root/aggregator/*.sv] [glob $rtl_root/isp/*.sv] [glob $rtl_root/ai_preproc/*.sv]

# Connect: Shakti Ganga M_AXI -> axi_ic -> camera S_AXI_Lite slaves
# Data: mipi_csi2_rx M_AXIS -> vc_merge -> isp -> ai_preproc -> AXI DMA -> DDR
# IRQ: camera interrupt -> Shakti PLIC

# Address map (see docs/register_map.md); assign once clocks/interconnect are wired
# assign_bd_address -offset 0x44A00000 -range 64K [get_bd_addr_segs {mipi_csi2_rx/S_AXI_LITE/Reg}]

# Constraints
add_files -fileset constrs_1 -norecurse "$ROOT/examples/arty_a7_100t/top_ganga_mipi.xdc"

# validate_bd_design needs the SoC clocking from the Shakti side (see reference above)
# validate_bd_design
# make_wrapper -files [get_files shakti_ganga_mipi.bd] -top -import
puts "{ok} BD skeleton created (clocks deferred to Shakti SoC integration)"
