# Week 03 Abstract Implementation Description

## Objective
The objective of Week 03 is to provide the first executable abstract implementation of the cross-traffic management concept. This implementation is based on the Week 02 UPPAAL model and demonstrates the control behavior before the later FreeRTOS/C implementation.

## Implemented System
The implementation models a four-way autonomous intersection controlled by an RSU. Each vehicle follows this communication and control flow:

1. The vehicle approaches the intersection.
2. The vehicle sends a V2V beacon containing its route intention.
3. The vehicle sends a V2I request to the RSU.
4. The RSU checks the current phase, server capacity, and route conflicts.
5. The RSU either grants the request or keeps the vehicle waiting.
6. The vehicle crosses the intersection after receiving a grant.
7. The vehicle sends a complete message to release the server slot.

## Queueing Modes

### M/M/1-style mode
The M/M/1-style mode is represented by:

```python
server_cap = 1
```

Only one vehicle can be inside the intersection service area at a time.

### M/M/2-style mode
The M/M/2-style mode is represented by:

```python
server_cap = 2
```

Two compatible vehicles can be served together. In this abstract implementation, compatible right-turn vehicles may cross together, while straight/conflicting routes are served conservatively.

## Main Implementation File

```text
src/abstract_cross_traffic_controller.py
```

This file contains:

- Vehicle data model
- RSU controller logic
- Phase controller logic
- V2V/V2I message simulation
- M/M/1 and M/M/2 execution modes
- Safety and metric collection

## Verification by Execution
The Python implementation is not a replacement for UPPAAL formal verification. Instead, it is the first executable mapping of the verified model. The logs in the `results/` folder show that:

- All vehicles can complete crossing.
- The server capacity is respected.
- M/M/1 does not allow simultaneous crossing.
- M/M/2 allows compatible simultaneous crossing.
- No conflict violation occurs in the tested scenario.
