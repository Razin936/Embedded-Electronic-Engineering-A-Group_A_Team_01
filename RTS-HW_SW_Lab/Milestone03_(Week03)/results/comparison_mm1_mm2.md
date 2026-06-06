# M/M/1 vs M/M/2 Abstract Implementation Comparison

## Goal
This comparison evaluates the first abstract implementation of the RSU-based cross-traffic controller in two queueing configurations.

| Feature | M/M/1 Mode | M/M/2 Mode |
|---|---|---|
| Server capacity | `SERVER_CAP = 1` | `SERVER_CAP = 2` |
| Service interpretation | One vehicle can be served at a time | Two compatible vehicles can be served at the same time |
| Simultaneous crossing | Not allowed | Allowed only for compatible right-turn routes |
| Conflict handling | Conservative, one active vehicle only | Conflict check blocks incompatible routes |
| V2V communication | Beacon before request | Beacon before request |
| V2I communication | Request, grant, complete | Request, grant, complete |
| RSU decision logic | Phase + capacity + conflict check | Phase + capacity + compatibility check |
| Throughput expectation | Lower | Higher for compatible traffic |
| Safety expectation | High due to single active slot | High due to route-conflict guard |
| Implementation complexity | Lower | Medium |

## Observed Abstract Simulation Metrics

| Metric | M/M/1 | M/M/2 |
|---|---:|---:|
| Completed vehicles | 4/4 | 4/4 |
| Total wait time | 10 | 8 |
| Average wait time | 2.50 | 2.00 |
| Maximum wait time | 6 | 6 |
| Total simulation time | 13 | 13 |
| Simultaneous crossing events | 0 | 2 |
| Conflict violations | 0 | 0 |

## Interpretation
The M/M/1 version is safer and simpler because only one vehicle can occupy the intersection at a time. However, it can increase waiting time because compatible vehicles cannot be served in parallel.

The M/M/2 version improves the service behavior for compatible routes. In this implementation, Vehicle 1 and Vehicle 2 represent compatible right-turn movements and can cross together. Straight/conflicting routes remain protected by the conflict guard, so safety is preserved while allowing limited parallelism.
