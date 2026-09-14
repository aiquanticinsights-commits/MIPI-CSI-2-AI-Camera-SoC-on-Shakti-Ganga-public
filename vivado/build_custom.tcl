# build_custom.tcl – Full custom bitstream path: Shakti SoC (Ganga) + camera overlay.
# Usage: vivado -mode batch -source vivado/build_custom.tcl   (from repo root)
# Prereqs: Vivado 2026.1+, BSC (Bluespec) for the Shakti BSV path, and the
# implementation RTL tree (commercial license). Without them this script
# explains what is missing and falls back to the prebuilt bitstream path.
if {[info exists ::env(BOARD_REPO)]} {
  set_param board.repoPaths $::env(BOARD_REPO)
} else {
  set_param board.repoPaths [file join $::env(APPDATA) Xilinx Vivado 2026.1 xhub board_store xilinx_board_store]
}

# Check BSC (only needed to rebuild the Shakti SoC itself from BSV)
set has_bsc 0
catch {exec bsc --version} bsc_ver
if {[info exists bsc_ver] && $bsc_ver ne ""} {set has_bsc 1; puts "BSC found: $bsc_ver"} else {puts "BSC not found - SoC rebuild unavailable; use the prebuilt bitstream (docs/quickstart.md)"}

# 1. Shakti Ganga SoC via a GC2025 checkout (set SHAKTI_GC2025 env var).
#    Expected: $SHAKTI_GC2025/hw/boards/arty_a7_ganga/tcl/env.tcl and
#    BSC-generated Verilog at $SHAKTI_GC2025/hw/build/verilog/mkSoc.v
if {[info exists ::env(SHAKTI_GC2025)]} {
  set shakti_hw [file join $::env(SHAKTI_GC2025) "hw"]
  set board "arty_a7_ganga"
  if {[file exists "$shakti_hw/boards/$board/tcl/env.tcl"]} {
    source "$shakti_hw/boards/$board/tcl/env.tcl"
    if {[file exists "$shakti_hw/build/verilog/mkSoc.v"]} {
      puts "Building full custom Shakti Ganga SoC from $shakti_hw/build/verilog"
      source "$shakti_hw/boards/$board/tcl/create_project.tcl" -tclargs "fpga_top" "xc7a100tcsg324-1" "RV64IMAFDCSU" "JTAG_EXTERNAL" "$shakti_hw/build/verilog" "64"
    } else {
      puts "No Shakti Verilog build found - run the GC2025 hw build first (requires BSC)"
    }
  }
} else {
  puts "SHAKTI_GC2025 not set - skipping SoC rebuild; use the prebuilt bitstream"
}

# 2. Overlay the camera block design
set SCRIPT_DIR [file dirname [file normalize [info script]]]
source "$SCRIPT_DIR/bd.tcl"
puts "{ok} Full custom build skeleton ready - launch_runs synth_1 for bitstream"
# launch_runs synth_1 -jobs 4; wait_on_run synth_1; launch_runs impl_1 -jobs 4
