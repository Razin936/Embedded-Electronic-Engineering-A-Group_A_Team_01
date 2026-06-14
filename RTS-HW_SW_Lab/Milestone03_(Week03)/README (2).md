# Week 3 — First Abstract Implementation

RSU-based autonomous cross-traffic intersection control. Abstract C core
logic, a one-to-one mapping of the verified UPPAAL timed-automata model
(`RSU_MM1_MM2_priority_emergency_intersection`).

## Files
- `intersection_rsu.c` — abstract implementation (single-threaded, deterministic).
- `week3_report.pdf` — Week 3 documentation (scenario, mapping, verification, results).

## Build & run
```bash
gcc -std=c11 -Wall -O2 intersection_rsu.c -o intersection_rsu
./intersection_rsu mm1     # M/M/1 — one vehicle in the conflict zone
./intersection_rsu mm2     # M/M/2 — two non-conflicting vehicles (default)
```

## What it reproduces from the model
- M/M/1 and M/M/2 server capacity (switch `MM_MODE`).
- Priority: emergency > main street > side street; right turn before left in a conflict.
- Emergency I2V yield broadcast + exclusive crossing.
- V2I (req/emg_req), I2V (grant/yield), V2V (enter/leave) message flow.
- Continuous (cyclic) arrival per vehicle period.
- Critical mutual exclusion: V4 (North-right) and V5 (South-left) never cross together.

## Run-time checks (= UPPAAL queries)
Asserted every tick: `noCollision()`, `crossingCount <= MM_MODE`,
`!(crossing[4] && crossing[5])`. Reachability (`E<> crossing[0]`,
`E<> crossing[1] && crossing[2]`) is confirmed in the event log and summary.

## Outlook
- Week 4: HW/SW codesign — offload the parallel conflict / `noCollision`
  evaluation to hardware (ModelSim/VHDL); keep admission logic in SW.
- Add a fairness/aging term to remove the observed starvation of the
  lowest-priority side-street left turn (V5).
