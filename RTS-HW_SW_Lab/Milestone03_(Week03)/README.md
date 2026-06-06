# Milestone 03 / Week 03 - First Abstract Implementation

## Topic
**Cross-Traffic Management for Autonomous Vehicles using Queueing-Based RSU Control**

## Purpose
This folder contains the first executable abstract implementation for Week 03. It converts the Week 02 UPPAAL model into a small Python controller prototype. The implementation focuses on the core logic before moving toward FreeRTOS/C implementation in later milestones.

## Implemented Features

- Four abstract vehicles approaching a four-way intersection.
- V2V intention beacon before requesting access.
- V2I request, grant, and complete message flow with the RSU.
- Phase-based RSU control:
  - `RIGHT_1`
  - `STRAIGHT_NS`
  - `RIGHT_2`
  - `STRAIGHT_EW`
- M/M/1-style mode using `SERVER_CAP = 1`.
- M/M/2-style mode using `SERVER_CAP = 2`.
- Conflict checking before granting intersection access.
- Deterministic simulation logs for GitHub evidence and report integration.
- Unit tests for completion, capacity, and safety behavior.


## How to Run

From the `milestone_03_week03` directory:

```bash
python src/abstract_cross_traffic_controller.py --mode mm1
python src/abstract_cross_traffic_controller.py --mode mm2
```

To write logs to files:

```bash
python src/abstract_cross_traffic_controller.py --mode mm1 --output results/mm1_simulation_log.txt
python src/abstract_cross_traffic_controller.py --mode mm2 --output results/mm2_simulation_log.txt
```

To run tests:

```bash
python tests/test_abstract_controller.py
```

## Main Result

The M/M/1 mode allows only one vehicle to cross at a time. The M/M/2 mode allows two compatible right-turn vehicles to cross together while still preventing conflicting routes from entering the intersection simultaneously. The logs and metrics in the `results/` folder document this behavior.
