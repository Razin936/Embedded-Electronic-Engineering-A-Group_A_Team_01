# Milestone 2: Scientific Contextualization and Tradeoff Analysis  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose

This milestone places list-based scheduling into the wider scientific and engineering context of High-Level Synthesis (HLS). The focus is not only to list related papers, but to compare approaches, assumptions, design alternatives, tradeoffs, and consequences for embedded systems and constrained hardware.

## 1. Position in the HLS Flow

A simplified HLS flow is:

```text
Algorithm → DFG/CDFG → Scheduling → Allocation → Binding → RTL
```

- Scheduling decides **when** operations execute.
- Allocation decides **how many** hardware resources are available.
- Binding decides **which operation uses which resource**.
- List-based scheduling mainly addresses the scheduling step.
- Its result still affects:
  - latency,
  - area,
  - number of functional units,
  - register usage,
  - controller complexity,
  - and power consumption.

## 2. Related Approaches and Neighboring Methods

| Approach | Main idea | Relation to list-based scheduling |
|---|---|---|
| ASAP scheduling | Schedule every operation as early as possible. | Baseline and useful for earliest start times. |
| ALAP scheduling | Schedule every operation as late as possible within a latency bound. | Useful for latest start times and mobility. |
| List-based scheduling | Use a ready list and priority rules under resource constraints. | Main topic. |
| Force-directed scheduling | Balance operation distribution over time. | More global than simple list scheduling. |
| ILP/exact scheduling | Encode scheduling as an optimization problem. | Can be optimal for small cases but may scale poorly. |
| Modulo/pipelined scheduling | Overlap loop iterations for throughput. | Important for loops and streaming designs. |
| Manual RTL design | Designer manually controls cycles/resources/registers. | More control, but more design effort. |
| HLS tools | Automate scheduling, allocation, binding, and RTL generation. | May use list scheduling or variants internally. |

## 3. Scientific Context

### Classical HLS research

- HLS research studies how to synthesize efficient hardware from high-level descriptions.
- Scheduling, allocation, and binding are central HLS problems.
- De Micheli is useful for digital synthesis, optimization, and architectural-level design.
- Gajski et al. are useful for HLS foundations and design representations.
- Paulin and Knight are useful because force-directed scheduling is a major alternative to simple list scheduling.

### Neighboring problem classes

- **Compiler instruction scheduling:** also schedules operations under dependencies and resources.
- **Real-time scheduling:** focuses on deadlines and task timing.
- **Resource-constrained project scheduling:** similar dependency/resource structure.
- **Hardware/software co-design:** decides whether work is implemented in software or hardware.
- **Loop pipelining and dataflow HLS:** focuses more on throughput and initiation interval.

## 4. Main Design Alternatives

### ASAP/ALAP only

- Very simple.
- Useful for estimating earliest/latest control steps.
- Not enough when resource constraints are strict.
- Example: ASAP may place two additions in the same cycle even if only one ALU exists.

### List-based scheduling

- Practical compromise between simplicity and useful schedule quality.
- Works step by step using a ready list.
- Uses priority rules such as:
  - critical path,
  - mobility,
  - number of successors,
  - operation latency,
  - resource urgency.
- Limitation: result depends strongly on the priority rule.

### Force-directed scheduling

- Tries to distribute operations over time.
- More global than normal list scheduling.
- Useful when resource sharing and balanced hardware usage are important.
- More complex to explain and implement.

### Exact optimization / ILP

- Scheduling can be written as an optimization problem.
- Can optimize latency or resource usage.
- Strong for small examples.
- Main weakness: complexity grows quickly.

### Loop pipelining / modulo scheduling

- Important for loops and streaming computations.
- Focuses on throughput.
- Important metric:

```text
II = Initiation Interval
```

- Low II means new loop iterations can start frequently.

## 5. Tradeoff Analysis

| Tradeoff | List-based scheduling advantage | Limitation / risk |
|---|---|---|
| Optimality vs feasibility | Fast and feasible for practical graphs. | Does not guarantee global optimum. |
| Runtime vs schedule quality | Faster than exact methods. | May give worse latency/resource usage. |
| Simplicity vs global reasoning | Easy to implement and explain. | Local decisions can cause later conflicts. |
| Area vs latency | More resources can reduce cycles. | More functional units increase area and power. |
| Determinism vs flexibility | Fixed priority rules are predictable. | Different rules can change the schedule. |
| Offline computation vs runtime adaptation | HLS scheduling is done before hardware generation. | Less adaptive than software scheduling. |
| Control simplicity vs performance | Simple schedules may create simpler controllers. | Aggressive schedules may need more registers/control. |
| Memory simplicity vs realism | Basic examples ignore memory conflicts. | Real HLS designs may be limited by memory ports. |

## 6. Assumptions Behind Different Approaches

### ASAP/ALAP

- Assumes dependency information is enough for initial timing estimation.
- Simplest form may ignore resource conflicts.

### List-based scheduling

- Assumes operations can be ordered by a useful priority function.
- Assumes resource availability is known.
- Assumes local greedy choices are acceptable.

### Force-directed scheduling

- Assumes operation distributions can guide balanced scheduling.
- Better when global resource balance is important.

### ILP/exact methods

- Assumes scheduling can be encoded accurately.
- Assumes solver time is acceptable.

### Loop pipelining

- Assumes repeated loop iterations.
- Requires attention to loop-carried dependencies and memory ports.

## 7. Consequences for Embedded Systems

Embedded systems often have limited:

- area,
- power,
- memory bandwidth,
- timing margin,
- and design time.

List-based scheduling is attractive because it is practical and understandable. It can show how adding one extra ALU changes latency. For small FPGA or constrained hardware, this tradeoff is very important.

For high-throughput signal processing or image processing, loop pipelining and dataflow scheduling may become more important than simple acyclic list scheduling.

## 8. Connection to My Implementation Direction

Example:

```text
z = (a+b) * (c+d) + e
```

With **1 ALU + 1 multiplier**:

```text
Cycle 1: v1 = a+b
Cycle 2: v2 = c+d
Cycle 3: v3 = v1*v2
Cycle 4: v4 = v3+e
```

Result:

```text
4 cycles
```

With **2 ALUs + 1 multiplier**:

```text
Cycle 1: v1 = a+b and v2 = c+d
Cycle 2: v3 = v1*v2
Cycle 3: v4 = v3+e
```

Result:

```text
3 cycles
```

Main tradeoff:

```text
More hardware resources → lower latency but higher area/cost
```

This can be shown using a schedule table, C++ output, or VHDL/Questa waveform.

## 9. Why Some Related Work Is Only Indirectly Related

- Some HLS papers focus on modern tool flows and pragmas, not scheduling theory.
- Some FPGA papers focus on placement/routing, which happens after RTL generation.
- Some real-time scheduling papers focus on software tasks, not operation-level hardware scheduling.
- Some accelerator papers use HLS but focus on architecture or application performance.
- Therefore, the final paper should use references that directly help explain scheduling, resource constraints, or HLS tradeoffs.

## 10. Selected References and Justification

| Reference | Why it is relevant |
|---|---|
| De Micheli | Foundational source for digital synthesis and optimization. |
| Gajski et al. | Foundational HLS textbook and useful for HLS flow. |
| Paulin and Knight | Important comparison point because of force-directed scheduling. |
| Coussy and Morawiec | Broader context from algorithm to digital circuit. |
| Canis et al. | Useful HLS tool-flow reference for FPGA accelerator systems. |

## 11. Excluded or Less Central References

- Pure FPGA place-and-route references are less central.
- General operating-system scheduling references are less central.
- Application-specific accelerator papers are only useful if they discuss HLS scheduling/resource constraints.
- Vendor manuals can support practical points, but scientific references should support the theory.

## 12. Milestone 2 Checklist Status

| Requirement | Status |
|---|---|
| Compared approaches or assumptions, not only summarized papers | Done |
| Identified neighboring approaches and design alternatives | Done |
| Explained direct/indirect/sparse related work | Done |
| Analyzed scientific and engineering tradeoffs | Done |
| Considered embedded-system constraints | Done |
| Justified selected references | Done |
| Justified excluded or less relevant references | Done |

## Short Summary

List-based scheduling is positioned between simple ASAP/ALAP timing methods and more complex approaches such as force-directed scheduling or exact optimization. Its main advantage is practical feasibility and understandable behavior. Its main weakness is that it is heuristic and priority-dependent. For embedded systems, the central tradeoff is latency versus area and power.
