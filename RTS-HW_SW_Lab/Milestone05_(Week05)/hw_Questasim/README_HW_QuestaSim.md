# Week 5 HW — QuestaSim Conflict & Safety Coprocessor

This folder contains the VHDL hardware partition for the RSU intersection controller.

## Files

- `conflict_safety_coprocessor.vhd` — synthesizable VHDL coprocessor.
- `tb_conflict_safety_coprocessor.vhd` — QuestaSim testbench with assertions.
- `run_console.do` — compile and run in console mode.
- `run_gui.do` — compile, run, and open wave window.
- `README_HW_QuestaSim.md` — this guide.

## Encoding

Vehicle IDs:

- V0 = emergency EAST straight
- V1 = main EAST straight
- V2 = main WEST straight
- V3 = side NORTH straight
- V4 = side NORTH right
- V5 = side SOUTH left

Origin encoding:

- EAST  = `00`
- WEST  = `01`
- NORTH = `10`
- SOUTH = `11`

Turn encoding:

- STRAIGHT = `00`
- RIGHT    = `01`
- LEFT     = `10`

Mode encoding:

- `0` = M/M/1
- `1` = M/M/2

## What the hardware computes

- `st_safe`  = `safe_to_grant(candidate)`
- `st_nocol` = `noCollision()`
- `st_cwc`   = `conflictsWithCrossing[N]`
- `st_alarm` = latched collision alarm

## Important tests

The testbench checks:

1. Empty M/M/2 allows V1.
2. V1 + V2 can cross together in M/M/2.
3. V1 + V2 crossing gives `st_cwc = 0x39`, matching the FreeRTOS software output.
4. V4 + V5 crossing is detected as unsafe.
5. Collision alarm latches and reset clears it.
6. M/M/1 blocks a second vehicle while one is crossing.
7. Emergency V0 waits while another vehicle is crossing and becomes safe after the intersection is free.

## How to run in QuestaSim

Open QuestaSim and change directory to this folder:

```tcl
cd C:/Users/shale/Documents/6th_sem/Embadded_system/Cross_section/Week_05/hw_Questasim
```

For GUI waveform:

```tcl
do run_gui.do
```

For console-only run:

```tcl
do run_console.do
```

Expected final console message:

```text
ALL TESTS PASSED: Conflict & Safety Coprocessor works
```

## Screenshots to save

Save these inside `hw_Questasim/result_png/`:

- `01_questasim_compile_success.png`
- `02_questasim_all_tests_passed.png`
- `03_questasim_waveform_safe_to_grant.png`
- `04_questasim_waveform_collision_alarm.png`
