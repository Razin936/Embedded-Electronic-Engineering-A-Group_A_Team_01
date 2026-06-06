# V2V and V2I Message Flow

## V2V Communication
The V2V part is represented as an intention beacon. Before requesting access from the RSU, each vehicle announces its route intention.

Example log format:

```text
t=00 | V2V | Vehicle1: BEACON(route=N2E, side=N)
```

Purpose:

- Shows that the vehicle can communicate its intention.
- Matches the Week 02 UPPAAL beacon abstraction.
- Prepares the design for later distributed or semi-distributed communication extensions.

## V2I Communication
The V2I part is represented by three messages.

### 1. Request

```text
Vehicle -> RSU: REQUEST(route, route_group)
```

The vehicle asks the RSU for permission to enter the intersection.

### 2. Grant

```text
RSU -> Vehicle: GRANT(activeCount, phase)
```

The RSU gives permission after checking:

- current phase
- server capacity
- route compatibility/conflict

### 3. Complete

```text
Vehicle -> RSU: COMPLETE(activeCount)
```

The vehicle informs the RSU that it has finished crossing. The RSU then releases the service slot.

## Message Flow Summary

```text
Vehicle approaches
    ↓
V2V beacon
    ↓
V2I request to RSU
    ↓
RSU checks phase, capacity, and conflict
    ↓
Grant or wait
    ↓
Vehicle crosses
    ↓
Vehicle sends complete
    ↓
RSU releases slot
```
