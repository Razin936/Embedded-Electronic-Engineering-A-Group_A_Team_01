# Implementation Limitations and Next Steps

## Current Limitations

This Week 03 implementation is an abstract software prototype. It focuses on the core controller behavior and communication flow rather than full vehicle dynamics or operating-system-level scheduling.

The current limitations are:

1. **No real network communication**  
   V2V and V2I are represented as log events and function calls.

2. **No stochastic arrival/service distribution yet**  
   The model uses deterministic arrival and crossing times. This is suitable for the first abstract implementation, but numerical queueing analysis can later add stochastic parameters.

3. **No FreeRTOS tasks yet**  
   The implementation is Python-based. FreeRTOS mapping is documented, but the actual C/FreeRTOS implementation belongs to a later milestone.

4. **Simplified route compatibility**  
   Compatible right-turn routes can cross together in M/M/2 mode. Straight routes are handled conservatively to keep the first implementation safe and aligned with the verified UPPAAL abstraction.

5. **No CARLA physics in this abstract file**  
   CARLA is used as a visual and scenario prototype separately. This implementation isolates the RSU controller logic.

## Next Steps

1. Add numeric M/M/1 and M/M/2 queueing analysis.
2. Extend vehicle arrivals to configurable traffic patterns.
3. Convert the RSU controller into C-code.
4. Map request/grant/complete messages to FreeRTOS queues and task notifications.
5. Integrate the abstract logic with the CARLA simulation controller.
6. Prepare a Week 04 HW/SW codesign proposal.
