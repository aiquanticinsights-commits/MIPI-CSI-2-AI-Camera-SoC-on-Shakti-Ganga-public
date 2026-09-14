# run_sim.tcl – XSIM behavioral simulation, Shakti Ganga MIPI pipeline.
# Usage: vivado -mode batch -source vivado/run_sim.tcl   (from repo root)
#
# Needs the implementation RTL tree (commercial license) at <root>/rtl
# with the documented block layout (csi2_rx, vc_merge, isp_pipeline,
# ai_preproc) plus examples/arty_a7_100t top/testbench. Without the RTL
# tree this script exits with a message instead of failing obscurely.
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
create_project -force sim_ganga ./sim_ganga -part xc7a100tcsg324-1
add_files -norecurse "$RTL/csi2_rx/csi2_rx.sv" "$RTL/aggregator/vc_merge.sv" "$RTL/isp/isp_pipeline.sv" "$RTL/ai_preproc/ai_preproc.sv"
add_files -norecurse "$EXDIR/top_ganga_mipi.sv" "$EXDIR/tb_top_ganga_mipi.sv"
set_property top tb_top_ganga_mipi [get_filesets sim_1]
set_property top_lib xil_defaultlib [get_filesets sim_1]
update_compile_order -fileset sim_1
launch_simulation -simset sim_1 -mode behavioral
run 20us
close_sim
puts "{ok} XSIM done"
