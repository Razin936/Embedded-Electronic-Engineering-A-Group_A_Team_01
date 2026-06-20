# QuestaSim / ModelSim GUI run script
# Usage in Transcript: do run_gui.do

transcript file transcript_hw_questasim.log

if {[file exists work]} {
    vdel -lib work -all
}

vlib work
vmap work work

vcom -2008 conflict_safety_coprocessor.vhd
vcom -2008 tb_conflict_safety_coprocessor.vhd

vsim -voptargs=+acc work.tb_conflict_safety_coprocessor
add wave -divider "Clock and Control"
add wave sim:/tb_conflict_safety_coprocessor/clk
add wave sim:/tb_conflict_safety_coprocessor/rst_n
add wave sim:/tb_conflict_safety_coprocessor/ctrl_start
add wave -divider "Register Inputs"
add wave sim:/tb_conflict_safety_coprocessor/mode
add wave sim:/tb_conflict_safety_coprocessor/cand
add wave -radix binary sim:/tb_conflict_safety_coprocessor/crossing
add wave -radix binary sim:/tb_conflict_safety_coprocessor/emergency
add wave -radix binary sim:/tb_conflict_safety_coprocessor/origin
add wave -radix binary sim:/tb_conflict_safety_coprocessor/turn
add wave -divider "Status Outputs"
add wave sim:/tb_conflict_safety_coprocessor/st_safe
add wave sim:/tb_conflict_safety_coprocessor/st_nocol
add wave -radix hexadecimal sim:/tb_conflict_safety_coprocessor/st_cwc
add wave sim:/tb_conflict_safety_coprocessor/st_alarm
run -all
wave zoom full
