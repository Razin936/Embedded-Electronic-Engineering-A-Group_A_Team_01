# Relation to the CARLA Prototype

The Week 03 abstract controller is not a replacement for the CARLA simulation. It is a smaller implementation of the core decision logic that can be tested and later transferred to C/FreeRTOS.

## CARLA Prototype Role
The CARLA prototype demonstrates the four-way intersection visually and includes:

- vehicles arriving from North, East, South, and West
- right and straight movement patterns
- queue behavior
- phase-based route release
- lane-safe paths
- active vehicle limits
- safe-distance handling

## Week 03 Abstract Implementation Role
The Week 03 implementation extracts the decision layer:

- V2V beacon before requesting access
- V2I request/grant/complete message flow
- RSU server-capacity check
- M/M/1 and M/M/2 control modes
- phase-based grant logic
- conflict-safe crossing decision

## Why This Split Is Useful
The CARLA model is useful for scenario visualization and traffic movement, while the abstract implementation is useful for showing the first software realization of the controller logic. This separation makes it easier to map the logic later to C-code and FreeRTOS tasks.
