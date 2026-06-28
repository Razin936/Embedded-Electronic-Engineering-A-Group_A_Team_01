# Milestone 1: Technical Understanding and Research Framing  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose
This milestone is a structured technical briefing. It is not written as a polished final paper. It uses bullet points, formulas, algorithm notes, assumptions, and open questions.

## Problem Statement and Motivation

- High-Level Synthesis (HLS) transforms an algorithmic or behavioral description into hardware.
- Scheduling decides in which clock cycle/control step each operation is executed.
- The main problem is that operations have dependencies and hardware resources are limited.
- If only one adder is available, two independent additions cannot run in the same cycle.
- Scheduling must balance:
  - low latency,
  - limited functional units,
  - dependency correctness,
  - hardware area,
  - timing/resource constraints.
- List-based scheduling is a constructive heuristic for producing a feasible schedule step by step.

## Core Technical Idea

- Input program is represented as a data-flow graph.
- Nodes are operations such as add, multiply, compare, or memory access.
- Edges are data dependencies.
- At each control step, the scheduler creates a ready list.
- An operation is ready if all predecessor operations are already scheduled.
- If too many operations are ready, priority rules decide which ones are scheduled first.

Typical priority criteria:

- critical path length,
- mobility,
- number of successors,
- operation latency,
- urgency caused by deadlines or latency constraints.

## Formal Model

Data-flow graph:

```text
G = (V, E)
```

- `V`: set of operations.
- `E`: set of dependency edges.
- `(u, v) in E`: operation `v` depends on result of `u`.
- `l(v)`: latency of operation `v`.
- `type(v)`: required resource type.
- `R_r`: number of available resources of type `r`.
- `t(v)`: scheduled start control step.

Precedence constraint:

```text
t(v) >= t(u) + l(u)
```

Resource constraint:

```text
number of active operations of resource type r at cycle c <= R_r
```

Schedule latency:

```text
L = max(t(v) + l(v) - 1)
```

## ASAP, ALAP, and Mobility

- ASAP: schedule operations as soon as possible.
- ALAP: schedule operations as late as possible under a latency bound.
- Mobility:

```text
mobility(v) = ALAP(v) - ASAP(v)
```

- Low mobility means urgent.
- High mobility means flexible.
- Common rule: schedule low-mobility or critical-path operations first.

## List-Based Scheduling Algorithm

```text
Input:
  Data-flow graph G=(V,E)
  Available resources R
  Priority rule P

1. Compute priorities.
2. Set current control step c = 1.
3. Find all ready operations.
4. Sort ready operations using priority rule P.
5. Schedule highest-priority ready operations while resources are available.
6. Move to next control step and repeat until all operations are scheduled.
```

## Small Example

Expression:

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

- Cycle 1: `v1 = a+b`
- Cycle 2: `v2 = c+d`
- Cycle 3: `v3 = v1*v2`
- Cycle 4: `v4 = v3+e`
- Result: 4-cycle schedule.

### Case B: 2 ALUs + 1 Multiplier

- Cycle 1: `v1 = a+b` and `v2 = c+d`
- Cycle 2: `v3 = v1*v2`
- Cycle 3: `v4 = v3+e`
- Result: 3-cycle schedule.

## Assumptions and Scope Limitations

- Program is represented as a static data-flow graph.
- Operation latencies are known before scheduling.
- Number of functional units is fixed.
- Clock period is assumed to be already chosen.
- Memory conflicts are not deeply modeled in the basic version.
- Branches and loops are not the main focus of this first milestone.
- Binding and allocation are related HLS steps, but the focus here is scheduling.
- List scheduling is a heuristic, so it does not always guarantee a globally optimal schedule.

## Runtime, Memory, Scalability, and Resource Aspects

- List scheduling is practical because it avoids solving a full global optimization problem.
- Main data structures:
  - data-flow graph,
  - ready list,
  - scheduled operation table,
  - resource availability table.
- Approximate memory usage:

```text
O(|V| + |E|)
```

- Approximate runtime:
  - graph traversal: about `O(|V| + |E|)`,
  - priority selection: about `O(|V| log |V|)` with a priority queue.
- More functional units can reduce latency but increase area and power.

## Relevance for Embedded Systems

- Embedded systems often have strict limits for area, power, timing, and memory.
- HLS helps convert high-level algorithms into hardware.
- Scheduling affects:
  - latency,
  - throughput,
  - number of adders, multipliers, and registers,
  - control logic complexity.
- List-based scheduling is useful because it gives a practical schedule under resource limits.

## Open Questions for Feedback

- Which priority rule should be emphasized: critical path, mobility, or number of successors?
- Should the final paper compare list scheduling with ASAP, ALAP, and force-directed scheduling?
- Should memory accesses be modeled as separate resources?
- Should multiplication be one-cycle or multi-cycle?
- Should the implementation evidence include C++ output, VHDL/Questa waveform, or both?
- Should the presentation show the schedule table or the data-flow graph first?

## Milestone 1 Checklist Status

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

List-based scheduling is a practical HLS scheduling method. It schedules ready operations step by step while respecting data dependencies and limited hardware resources. Its main strength is simplicity and scalability. Its main weakness is that the result depends strongly on priority rules and may not be globally optimal.