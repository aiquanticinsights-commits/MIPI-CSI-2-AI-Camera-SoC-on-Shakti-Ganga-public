# run_bitstream.tcl – synth + impl + bitstream, Shakti Ganga MIPI (Arty A7-100T).
# Usage: vivado -mode batch -source vivado/run_bitstream.tcl   (from repo root)
# Prereqs: Vivado 2026.1+, Digilent arty-a7-100 board files, implementation
# RTL tree (commercial license) at <root>/rtl. Reproduces the verified
# ganga_mipi_top.bit (see docs/BITSTREAM_GANGA.md for the reference numbers).
if {[info exists ::env(BOARD_REPO)]} {
  set_param board.repoPaths $::env(BOARD_REPO)
} else {
  set_param board.repoPaths [file join $::env(APPDATA) Xilinx Vivado 2026.1 xhub board_store xilinx_board_store]
}
set SCRIPT_DIR [file dirname [file normalize [info script]]]
set ROOT [file normalize "$SCRIPT_DIR/.."]
set RTL "$ROOT/rtl"
set EXDIR "$ROOT/examples/arty_a7_100t"
if {![file isdirectory $RTL]} {
  puts "RTL tree not found at $RTL."
  puts "This public release ships docs, SDK and bitstream; the implementation"
  puts "RTL is available under commercial license (see README.md)."
  exit 2
}
create_project -force ganga_mipi ./ganga_mipi -part xc7a100tcsg324-1
set_property board_part digilentinc.com:arty-a7-100:part0:1.1 [current_project]
add_files -norecurse "$RTL/csi2_rx/csi2_rx.sv" "$RTL/aggregator/vc_merge.sv" "$RTL/isp/isp_pipeline.sv" "$RTL/ai_preproc/ai_preproc.sv"
add_files -norecurse "$EXDIR/top_ganga_mipi.sv"
add_files -fileset constrs_1 -norecurse "$EXDIR/top_ganga_mipi.xdc"
set_property top top_ganga_mipi [get_filesets sources_1]
update_compile_order -fileset sources_1
launch_runs synth_1 -jobs 4
wait_on_run synth_1
puts "{ok} synth done"
launch_runs impl_1 -jobs 4
wait_on_run impl_1
puts "{ok} impl done"
open_run impl_1
report_utilization -file ganga_util.rpt
report_timing_summary -file ganga_timing.rpt
write_bitstream -force ./ganga_mipi_top.bit
puts "{ok} BITSTREAM ./ganga_mipi_top.bit"
