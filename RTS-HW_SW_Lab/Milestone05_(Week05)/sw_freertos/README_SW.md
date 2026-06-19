# Week 5 `sw_freertos` files

This folder contains the FreeRTOS software partition of the RSU intersection controller.

## Files

- `main.c` — creates queues, mutex, RSU task, vehicle tasks, starts scheduler.
- `vehicle_tasks.c` — six vehicle tasks V0–V5; request, wait grant, cross, leave.
- `rsu_controller.c` — RSU scheduling logic: queue, emergency priority, aging fairness, assertions.
- `hw_interface.c` — software stub of the future VHDL Conflict & Safety Coprocessor.
- `rsu_common.h` — shared types, constants, prototypes.

## Important switch

In `main.c`:

```c
MMMode_t g_mmMode = MODE_MM2;
```

Change to `MODE_MM1` if you want one vehicle at a time.

## What to show in Week 5

1. FreeRTOS task structure: Vehicle tasks + RSU task.
2. Queue messages: request, grant, leave.
3. `hw_safe_to_grant(i)` call inside `can_grant()`.
4. V1 + V2 can overlap in M/M/2.
5. V4 + V5 never overlap.
6. V0 emergency gets priority.
7. Aging avoids V5 starvation.
