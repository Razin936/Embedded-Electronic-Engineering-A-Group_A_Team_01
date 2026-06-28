# Milestone 2: Scientific Contextualization and Tradeoff Analysis  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose

This milestone places list-based scheduling into the wider scientific and engineering context of High-Level Synthesis (HLS).  
The focus is not only to name related work, but to compare approaches, assumptions, design alternatives, tradeoffs, and consequences for embedded systems and constrained hardware.  
The document is written as structured technical notes rather than polished final-paper prose.

## 1. Position of List-Based Scheduling in the HLS Flow

A simplified HLS flow is:

```text
Algorithm → DFG/CDFG → Scheduling → Allocation → Binding → RTL
```

- Scheduling decides **when** operations execute.
- Allocation decides **how many** hardware resources are available.
- Binding decides **which operation uses which resource**.
- List-based scheduling mainly addresses the scheduling step.
- However, scheduling decisions strongly affect:
  - latency,
  - area,
  - number of functional units,
  - register usage,
  - controller complexity,
  - power consumption.

## 2. Related Approaches and Neighboring Methods

| Approach | Main idea | Relation to list-based scheduling |
|---|---|---|
| ASAP scheduling | Schedule every operation as early as possible. | Baseline method and useful for earliest start times. |
| ALAP scheduling | Schedule every operation as late as possible within a latency bound. | Useful for latest start times and mobility calculation. |
| List-based scheduling | Maintain a ready list and schedule operations according to priority while respecting resources. | Main topic; practical heuristic for resource-constrained scheduling. |
| Force-directed scheduling | Use distribution/force concepts to balance operation placement over time. | More global than simple list scheduling, but more complex. |
| ILP/exact scheduling | Formulate scheduling as an optimization problem. | Can be optimal for small cases, but may scale poorly. |
| Modulo/pipelined scheduling | Overlap loop iterations to improve throughput. | Important for loops and streaming applications. |
| Manual RTL design | Designer explicitly controls cycles, resources, and registers. | High control, but more hardware design effort. |
| Commercial/open-source HLS tools | Automate scheduling, allocation, binding, and RTL creation. | May internally use list scheduling or similar heuristics. |

## 3. Scientific Context

### Classical HLS research

- Classical HLS research studies how efficient hardware can be derived from high-level descriptions.
- Scheduling, allocation, and binding are central problems in HLS.
- De Micheli is a foundational source for synthesis and optimization of digital circuits.
- Gajski et al. are useful for HLS foundations, design representations, scheduling, allocation, and system-level design concepts.
- Paulin and Knight introduced force-directed scheduling, which is important for comparison because it is a more global scheduling approach than simple list scheduling.

### Neighboring problem classes

- **Compiler instruction scheduling:** also schedules operations under dependency and resource constraints.
- **Real-time scheduling:** focuses on tasks, deadlines, and timing guarantees.
- **Resource-constrained project scheduling:** has a similar graph/resource structure, but is not hardware-specific.
- **Hardware/software co-design:** studies whether work should be implemented in software, hardware, or accelerators.
- **Loop pipelining and dataflow HLS:** focus more on throughput, initiation interval, and streaming behavior.

## 4. Main Design Alternatives

### ASAP and ALAP scheduling

- ASAP and ALAP are simple and useful for timing estimation.
- ASAP gives the earliest possible schedule.
- ALAP gives the latest possible schedule under a latency bound.
- They are useful for calculating mobility:

```text
mobility(v) = ALAP(v) - ASAP(v)
```

- Limitation: they are not sufficient when hardware resources are strictly limited.
- Example: ASAP may place two additions in the same cycle even if only one ALU is available.

### List-based scheduling

- Practical compromise between simplicity and useful scheduling quality.
- Works step by step using a ready list.
- Can include different priority rules:
  - critical path,
  - mobility,
  - number of successors,
  - operation latency,
  - resource urgency.
- Main limitation: the result depends strongly on the chosen priority rule.

### Force-directed scheduling

- Tries to distribute operations over time to reduce resource peaks.
- More global than normal list scheduling.
- Useful when resource sharing and balanced hardware usage are important.
- More complex to explain and implement than simple list scheduling.

### Exact optimization, for example ILP

- Scheduling can be formulated as an optimization problem.
- It can optimize objectives such as minimum latency or minimum resource usage.
- Advantage: can provide optimal results for small examples.
- Weakness: complexity increases quickly with graph size, resource types, and timing constraints.

### Loop pipelining / modulo scheduling

- Important for loops and streaming computations.
- Objective is often throughput, not only total latency.
- Key metric:

```text
II = Initiation Interval
```

- A low initiation interval means new loop iterations can start frequently.
- This is related to scheduling, but it is different from simple list scheduling for an acyclic expression graph.

## 5. Tradeoff Analysis

| Tradeoff | List-based scheduling advantage | Limitation / risk |
|---|---|---|
| Optimality vs feasibility | Fast and feasible for practical graphs. | Does not guarantee a globally optimal schedule. |
| Runtime vs schedule quality | Much faster than many exact methods. | May produce worse latency or resource usage than optimal methods. |
| Simplicity vs global reasoning | Easy to understand and implement. | Local priority decisions can cause later conflicts. |
| Area vs latency | More resources can reduce cycles. | More functional units increase area and power. |
| Determinism vs flexibility | Fixed priority rules give predictable behavior. | Changing priority rules can change the final schedule. |
| Offline computation vs runtime adaptation | HLS scheduling is normally done before hardware implementation. | Less adaptable to runtime changes than software scheduling. |
| Control simplicity vs performance | Simple schedules often produce simpler controllers. | Aggressive schedules may require more complex control and registers. |
| Memory simplicity vs realism | Basic DFG examples ignore many memory conflicts. | Real HLS designs often depend strongly on memory ports and arrays. |

## 6. Assumptions Behind Different Approaches

### ASAP/ALAP

- Assumes dependency information is enough for initial timing estimation.
- May ignore detailed resource conflicts in the simplest form.

### List-based scheduling

- Assumes operations can be ordered using a useful priority function.
- Assumes resource availability is known.
- Assumes local greedy decisions are acceptable.

### Force-directed scheduling

- Assumes operation distributions and probabilities can guide balanced scheduling.
- Useful when global resource balance is important.

### ILP/exact methods

- Assume the scheduling problem can be encoded accurately.
- Assume available solver time is acceptable.

### Loop pipelining

- Assumes repeated loop iterations and throughput-oriented optimization.
- Requires attention to loop-carried dependencies and memory ports.

## 7. Consequences for Embedded Systems and Constrained Hardware

Embedded systems often have limited:

- chip area,
- power budget,
- memory bandwidth,
- timing margin,
- design time.

List-based scheduling is attractive because it is practical and understandable.

It can show how adding one extra ALU changes latency.

For small FPGA or embedded accelerator designs, resource constraints are very important.

For safety-critical or hard real-time embedded systems, deterministic timing and explainable design decisions are important.

For high-throughput signal processing or image processing, loop pipelining and dataflow scheduling may be more important than simple acyclic list scheduling.

## 8. Connection to My Implementation Direction

Example:

```text
z = (a+b) * (c+d) + e
```

With **1 ALU + 1 multiplier**:

```text
L = 4 cycles
```

With **2 ALUs + 1 multiplier**:

```text
L = 3 cycles
```

Main tradeoff:

```text
More hardware resources → lower latency but higher area/cost
```

This implementation example is useful because it makes the scheduling tradeoff visible in a schedule table, C++ output, or Questa/VHDL waveform.

## 9. Why Some Related Work Is Only Indirectly Related

- Some HLS papers focus mainly on modern tool flows, pragmas, or compiler infrastructure instead of the scheduling algorithm itself.
- Some FPGA papers focus on placement/routing or bitstream generation, which happens after RTL creation and is therefore less directly related.
- Some real-time scheduling papers focus on task deadlines rather than operation-level hardware scheduling.
- Some machine-learning accelerator papers use HLS, but their main contribution may be architecture or dataflow design, not list scheduling.
- Therefore, the final paper should use references that help explain scheduling, resource constraints, or HLS tradeoffs directly.

## 10. Selected References and Justification

| Reference | Why it is relevant |
|---|---|
| De Micheli | Foundational source for digital synthesis, optimization, scheduling, and architectural-level design. |
| Gajski et al. | Foundational HLS source; useful for explaining the HLS flow and basic scheduling concepts. |
| Paulin and Knight | Important related method because force-directed scheduling is a major alternative to simple list scheduling. |
| Coussy and Morawiec | Useful broader HLS context from algorithm to digital circuit. |
| Canis et al. | Useful HLS tool-flow reference showing HLS relevance for FPGA processor/accelerator systems. |

## 11. Excluded or Less Central References

- Pure FPGA place-and-route references are less central because list scheduling happens earlier in the HLS flow.
- General operating-system scheduling references are less central because they schedule software tasks, not hardware operations in a DFG.
- Application-specific accelerator papers are useful only if they clearly discuss HLS scheduling or resource constraints.
- Vendor manuals can be useful for practical tools, but the seminar paper should mainly use scientific references for the core theory.

## 12. Milestone 2 Checklist Status

| Requirement | Status |
|---|---|
| Compared approaches or assumptions, not only summarized papers | Done |
| Identified neighboring approaches and design alternatives | Done |
| Explained why related work can be direct, indirect, or sparse | Done |
| Analyzed scientific and engineering tradeoffs | Done |
| Considered embedded-system constraints | Done |
| Justified selected references | Done |
| Justified excluded or less relevant references | Done |

## Short Summary

List-based scheduling is positioned between very simple ASAP/ALAP timing methods and more complex methods such as force-directed scheduling or exact optimization.  
Its main advantage is practical feasibility and understandable scheduling behavior.  
Its main weakness is that it is heuristic and priority-dependent.  
For embedded systems, the most important tradeoff is usually latency versus area and power.
