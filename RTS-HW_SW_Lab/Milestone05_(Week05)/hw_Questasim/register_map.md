# HW/SW Register Map — Conflict & Safety Coprocessor

| Register | Direction | Width | VHDL signal | Meaning |
|---|---:|---:|---|---|
| `STATE_ORIGIN` | SW -> HW | 12 bit | `origin` | 2-bit origin per vehicle |
| `STATE_TURN` | SW -> HW | 12 bit | `turn` | 2-bit turn per vehicle |
| `STATE_EMERGENCY` | SW -> HW | 6 bit | `emergency` | emergency flag per vehicle |
| `STATE_CROSSING` | SW -> HW | 6 bit | `crossing` | crossing flag per vehicle |
| `MODE` | SW -> HW | 1 bit | `mode` | 0=M/M/1, 1=M/M/2 |
| `CAND` | SW -> HW | 3 bit | `cand` | candidate vehicle ID |
| `CTRL` | SW -> HW | 1 bit | `ctrl_start` | latch/check alarm |
| `ST_SAFE` | HW -> SW | 1 bit | `st_safe` | safe_to_grant(candidate) |
| `ST_NOCOL` | HW -> SW | 1 bit | `st_nocol` | noCollision() |
| `ST_CWC` | HW -> SW | 6 bit | `st_cwc` | conflictsWithCrossing vector |
| `ST_ALARM` | HW -> SW | 1 bit | `st_alarm` | latched collision alarm |
