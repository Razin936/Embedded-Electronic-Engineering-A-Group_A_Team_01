# Milestone 1: Technical Understanding and Research Framing  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose

This document is a structured technical briefing for Milestone 1.  
It is written using concise technical notes, bullet points, formulas, pseudocode, assumptions, and open questions.  
The goal is to show technical understanding of the assigned topic before writing the final seminar paper.

## 1. Problem Statement and Motivation

- High-Level Synthesis (HLS) transforms a high-level algorithmic description into a hardware implementation.
- During HLS, **scheduling** decides in which clock cycle or control step each operation is executed.
- The main problem is that operations have data dependencies and hardware resources are limited.
- Example: if only one adder is available, two independent additions cannot execute in the same cycle.
- Scheduling must balance:
  - low latency,
  - limited functional units,
  - correct data dependency order,
  - small hardware area,
  - low resource cost,
  - predictable timing.
- **List-based scheduling** is a practical heuristic that builds a feasible schedule step by step.

## 2. Core Technical Idea

- The input program is represented as a **data-flow graph (DFG)** or a control/data-flow graph (CDFG).
- Nodes represent operations such as addition, multiplication, comparison, load, or store.
- Edges represent data dependencies between operations.
- At each control step, the scheduler creates a list of **ready operations**.
- An operation is ready when all predecessor operations have already been scheduled and their results are available.
- If more operations are ready than available hardware resources, a priority rule decides which operations are scheduled first.

Typical priority rules:

- longest critical path first,
- lowest mobility first,
- highest number of successors first,
- longest operation latency first,
- most urgent resource demand first.

## 3. Formal Model

Data-flow graph:

```text
G = (V, E)
```

- `V`: set of operations.
- `E`: set of dependency edges.
- `(u, v) in E`: operation `v` depends on result of `u`.
- `l(v)`: latency of operation `v`.
- `type(v)`: required resource type of operation `v`.
- `R_r`: number of available resources of type `r`.
- `t(v)`: scheduled start control step of operation `v`.

### Precedence Constraint

```text
t(v) >= t(u) + l(u)
```

Meaning: operation `v` can only start after operation `u` has finished.

### Resource Constraint

```text
number of active operations of resource type r at cycle c <= R_r
```

Meaning: at any control step, the number of active operations of one resource type must not exceed the available hardware units.

### Schedule Latency

```text
L = max(t(v) + l(v) - 1)
```

Meaning: the total schedule length is determined by the last finishing operation.

## 4. ASAP, ALAP, and Mobility

- **ASAP scheduling:** schedules each operation as soon as possible while respecting data dependencies.
- **ALAP scheduling:** schedules each operation as late as possible under a latency bound.
- **Mobility:** shows how much freedom an operation has.

```text
mobility(v) = ALAP(v) - ASAP(v)
```

- Low mobility means the operation is urgent.
- High mobility means the operation can be moved more freely.
- A common list-scheduling priority is to schedule low-mobility or critical-path operations first.

## 5. List-Based Scheduling Algorithm

```text
Input:
  Data-flow graph G=(V,E)
  Available resources R
  Priority rule P

Step 1:
  Compute operation priorities
  Example: critical path length, mobility, or number of successors

Step 2:
  Set current control step c = 1

Step 3:
  Find all ready operations
  An operation is ready if all predecessors are already scheduled

Step 4:
  Sort the ready operations using priority rule P

Step 5:
  Schedule the highest-priority ready operations
  only while the required resources are available

Step 6:
  Move to the next control step

Step 7:
  Repeat until all operations are scheduled
```

## 6. Small Example for My Topic

Example expression:

```text
z = (a+b) * (c+d) + e
```

| Operation | Meaning | Resource | Dependency |
|---|---|---|---|
| v1 | a+b | ALU | none |
| v2 | c+d | ALU | none |
| v3 | v1*v2 | Multiplier | v1, v2 |
| v4 | v3+e | ALU | v3 |

### Case A: 1 ALU + 1 Multiplier

- `v1` and `v2` are independent.
- However, only one ALU is available.
- Therefore, both additions cannot be executed in the same cycle.

A possible schedule:

```text
Cycle 1: v1 = a+b
Cycle 2: v2 = c+d
Cycle 3: v3 = v1*v2
Cycle 4: v4 = v3+e
```

Result:

```text
L = 4 cycles
```

### Case B: 2 ALUs + 1 Multiplier

- `v1` and `v2` can run in parallel because two ALUs are available.

A possible schedule:

```text
Cycle 1: v1 = a+b and v2 = c+d
Cycle 2: v3 = v1*v2
Cycle 3: v4 = v3+e
```

Result:

```text
L = 3 cycles
```

### Observation

```text
More hardware resources → lower latency
```

However:

```text
More hardware resources → higher area and possible power cost
```

## 7. Assumptions and Scope Limitations

- The program can be represented as a static data-flow graph.
- Operation latencies are known before scheduling.
- The number of functional units is fixed.
- The clock period is assumed to be already chosen.
- Basic examples often ignore memory access conflicts.
- Branches, loops, and function calls are not deeply analyzed in this first milestone.
- Binding and allocation are related HLS steps, but this milestone focuses mainly on scheduling.
- List-based scheduling is a heuristic; it usually produces a feasible schedule but does not always guarantee the globally optimal schedule.

## 8. Runtime, Memory, Scalability, and Resource Aspects

- List-based scheduling is practical because it avoids solving a full global optimization problem.
- Main data structures:
  - data-flow graph,
  - ready list,
  - scheduled operation table,
  - resource availability table,
  - priority values.

Approximate memory usage:

```text
O(|V| + |E|)
```

Approximate runtime depends on implementation:

- graph traversal: about `O(|V| + |E|)`,
- priority selection: often about `O(|V| log |V|)` with a priority queue.

Scalability issue:

- for very large graphs, priority choice, memory conflicts, and resource constraints become more important.

Hardware tradeoff:

- more functional units can reduce latency,
- but more functional units increase area, routing complexity, and possibly power consumption.

## 9. Why This Topic Is Relevant for Embedded Systems

- Embedded systems often have strict limits for area, power, timing, and memory.
- HLS helps designers move from high-level algorithm descriptions to hardware implementations.
- Scheduling directly affects:
  - execution latency,
  - throughput,
  - required number of adders, multipliers, registers, and multiplexers,
  - control logic complexity.
- List-based scheduling is useful because it gives a practical way to generate schedules under resource constraints.
- It is also easy to demonstrate using C++ output, schedule tables, and VHDL/Questa waveforms.

## 10. Unclear Points and Questions for Feedback

- Which priority rule should be emphasized in the final paper: critical path, mobility, or number of successors?
- Should the final paper compare list scheduling with ASAP, ALAP, and force-directed scheduling?
- Should memory accesses be modeled as separate resources?
- Should operation latencies be one-cycle only, or should multi-cycle multiplication be included?
- Should the implementation evidence include C++ output, VHDL/Questa waveform, or both?
- Should the final presentation show the data-flow graph first or the schedule table first?

## 11. Milestone 1 Checklist Status

| Requirement | Status |
|---|---|
| Technical problem stated in own words | Done |
| Main contribution explained | Done |
| Central algorithm/formalism identified | Done |
| Assumptions and scope limitations identified | Done |
| Runtime, memory, scalability, and resources considered | Done |
| Unclear points documented | Done |
| Relevance for embedded systems explained | Done |

## Short Summary

List-based scheduling is a practical HLS scheduling method.  
It schedules ready operations step by step while respecting data dependencies and limited hardware resources.  
Its main strength is simplicity and scalability.  
Its main weakness is that the final schedule depends strongly on the chosen priority rule and may not be globally optimal.
