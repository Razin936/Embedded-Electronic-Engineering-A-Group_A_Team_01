# Model-to-Implementation Mapping

This document explains how the Week 02 UPPAAL model was mapped into the Week 03 abstract Python implementation.

| UPPAAL / Model Element | Week 03 Python Element | Meaning |
|---|---|---|
| `Vehicle1_Template` to `Vehicle4_Template` | `VehiclePlan` and `VehicleRuntime` | Abstract autonomous vehicles |
| `RSU_Controller` | `RSUController` class | Central intersection controller |
| `Phase_Controller` | `Phase` enum and `update_phase()` | Phase rotation logic |
| `beacon1!`, `beacon2!`, ... | `send_beacon_and_request()` log event | V2V intention broadcast |
| `request1!`, `request2!`, ... | `receive_request()` | V2I request from vehicle to RSU |
| `grant1!`, `grant2!`, ... | `grant()` | V2I permission from RSU to vehicle |
| `complete1!`, `complete2!`, ... | `complete()` | Vehicle releases intersection slot |
| `SERVER_CAP = 1` | `mode="mm1"` | M/M/1-style single-server abstraction |
| `SERVER_CAP = 2` | `mode="mm2"` | M/M/2-style two-server abstraction |
| `activeCount` | `len(self.active)` | Number of vehicles currently crossing |
| Phase `RIGHT_1` / `RIGHT_2` | `Phase.RIGHT_1`, `Phase.RIGHT_2` | Right-turn service phase |
| Phase `STRAIGHT_NS` | `Phase.STRAIGHT_NS` | North-South straight phase |
| Phase `STRAIGHT_EW` | `Phase.STRAIGHT_EW` | East-West straight phase |
| Conflict guard | `no_conflict_with_active()` | Prevents unsafe simultaneous crossings |

## Mapping to CARLA Prototype
The abstract implementation keeps the same control idea as the CARLA prototype: phase-based release, queue/waiting behavior, and route-based movement groups. CARLA provides the visual simulation and motion behavior, while the Week 03 implementation isolates the core controller logic in a small executable model.

## Mapping to Future FreeRTOS Implementation

| Week 03 Python Element | Future FreeRTOS Element |
|---|---|
| `RSUController` | RSU control task |
| `VehicleRuntime` | Vehicle task or vehicle data structure |
| Waiting queue | FreeRTOS queue |
| `server_cap` | Counting semaphore |
| V2I request | Queue message to RSU task |
| V2I grant | Task notification or response queue |
| V2I complete | Completion queue message |
| Phase update | Software timer or periodic phase task |
| Safety guard | Critical section / protected controller logic |
