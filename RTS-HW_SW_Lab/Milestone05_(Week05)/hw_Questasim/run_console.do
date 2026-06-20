# QuestaSim / ModelSim console run script
# Usage in Transcript: do run_console.do

transcript file transcript_hw_questasim.log

if {[file exists work]} {
    vdel -lib work -all
}

vlib work
vmap work work

vcom -2008 conflict_safety_coprocessor.vhd
vcom -2008 tb_conflict_safety_coprocessor.vhd

vsim -c -voptargs=+acc work.tb_conflict_safety_coprocessor -do "run -all; quit -f"
