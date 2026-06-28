# Milestone 3: Critical Evaluation and Transfer  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose

This milestone critically evaluates list-based scheduling for High-Level Synthesis (HLS).  
The goal is not to describe the method as simply good or bad.  
The goal is to identify concrete strengths, limitations, hidden assumptions, risks, scalability issues, and possible transfer to realistic embedded-system and FPGA design contexts.  
The document is written as structured technical notes rather than polished final-paper prose.

## 1. Object of Evaluation

- **Topic:** List-Based Scheduling for High-Level Synthesis.
- **Main technical idea:** schedule ready operations step by step while respecting data dependencies and limited hardware resources.
- **Input model:** usually a data-flow graph (DFG) or control/data-flow graph (CDFG).
- **Main scheduling decision:** choose which ready operation should be scheduled when several operations compete for limited resources.

Typical priority rules:

- critical path length,
- mobility,
- number of successors,
- operation latency,
- resource urgency.

## 2. Evaluation Criteria

| Criterion | Evaluation question |
|---|---|
| Correctness | Does the schedule respect data dependencies and resource limits? |
| Latency | Does the method produce short schedules? |
| Resource usage | Does the method use limited adders, multipliers, registers, and memory ports effectively? |
| Runtime | Is the scheduling algorithm fast enough for practical HLS flows? |
| Scalability | Does it still work for larger graphs and realistic applications? |
| Predictability | Is the result understandable and deterministic? |
| Implementation effort | Is the method easy to implement and explain? |
| Transferability | Can the method be used in realistic embedded-system or FPGA design contexts? |

## 3. Concrete Strengths

### Strength 1: Simple and understandable

```text
ready list → priority selection → resource check → schedule
```

- Easy to explain.
- Suitable for seminar presentations and first HLS implementations.
- Easy to connect to schedule tables, C++ output, and VHDL/Questa waveforms.

### Strength 2: Practical runtime

- Usually much faster than exact optimization methods.
- Avoids solving a complete global optimization problem.
- Approximate graph memory usage:

```text
O(|V| + |E|)
```

- Practical for many medium-size HLS problems.

### Strength 3: Supports resource-constrained design

- Naturally handles limited functional units.
- Shows what happens when only one ALU is available compared with two ALUs.
- Important in embedded systems because area, power, and cost are often limited.

### Strength 4: Flexible priority rules

- The same framework can use different priority rules.
- Critical-path priority can reduce latency.
- Mobility-based priority can schedule urgent operations first.
- Successor-based priority can keep later graph operations active.

### Strength 5: Good HLS design intuition

```text
hardware resources ↔ latency ↔ area
```

- Explains why adding hardware can reduce latency.
- Supports engineering reasoning and design-space exploration.

## 4. Concrete Limitations and Risks

### Limitation 1: Not globally optimal

- List-based scheduling is a heuristic.
- It makes local decisions at each control step.
- A locally good decision can create a worse schedule later.
- It cannot generally guarantee minimum latency or minimum resource usage.

### Limitation 2: Strong dependence on the priority rule

- Different priority rules can produce different schedules.
- Critical-path priority, mobility priority, and successor priority may lead to different results.
- The final result depends strongly on the chosen rule.

### Limitation 3: Memory conflicts are often simplified

Simple DFG examples usually contain only additions and multiplications.

Real HLS designs often include:

- array reads,
- array writes,
- memory ports,
- buses,
- load/store dependencies.

Memory ports can become the real bottleneck even if enough ALUs are available.

### Limitation 4: Branches and loops are more difficult

Acyclic expression graphs are easier than real programs.

Real programs may include:

- branches,
- loops,
- function calls,
- loop-carried dependencies.

For loops, throughput and initiation interval may be more important than simple total latency.

### Limitation 5: Scheduling alone does not guarantee timing closure

A schedule may be valid at the HLS level but still fail later during FPGA or ASIC implementation.

Real hardware timing depends on:

- clock period,
- routing delay,
- functional unit delay,
- register placement,
- synthesis and place-and-route results.

## 5. Hidden or Restrictive Assumptions

| Assumption | Why it can be problematic |
|---|---|
| Operation latencies are known | Actual delay may depend on synthesis, device, and clock period. |
| Resources are fixed and simple | Real resources may have different delays, sharing costs, and pipeline behavior. |
| Dependencies are fully known | Pointers, arrays, and memory aliasing can make dependency analysis harder. |
| One operation uses one resource type | Some operations may require multiple resources, memory access, or control support. |
| Memory is simple | Real memory has limited ports and access conflicts. |
| Local priority is enough | Local greedy scheduling may miss better global schedules. |
| Acyclic DFG is representative | Many real applications contain loops, branches, and streaming behavior. |
| Area cost is simple | Extra resources also affect registers, multiplexers, routing, and power. |

## 6. Runtime, Memory, and Scalability

### Runtime

Graph traversal is approximately:

```text
O(|V| + |E|)
```

Priority selection may require sorting or a priority queue:

```text
O(|V| log |V|)
```

This is usually acceptable for HLS compared with exact optimization approaches.

### Memory

Main memory structures:

- graph nodes,
- dependency edges,
- ready list,
- schedule table,
- resource-availability table,
- priority values.

Approximate memory cost:

```text
O(|V| + |E|)
```

### Scalability

List scheduling scales better than exact scheduling for many practical designs.

However, schedule quality can decrease when:

- the graph is very large,
- many operations compete for the same resource,
- memory dependencies dominate,
- loops and branches are complex,
- the priority rule does not match the real design objective.

## 7. Suitable Application Examples

| Suitable case | Reason |
|---|---|
| Small arithmetic accelerators | Dependencies and resources are easy to model. |
| DSP-style datapaths | Adders, multipliers, and registers can be scheduled clearly. |
| Educational HLS examples | The method is explainable and visible in schedule tables. |
| FPGA student projects | Resource constraints are important and easy to demonstrate. |
| Medium-size acyclic DFGs | Practical scheduling can be obtained without complex solvers. |
| Early design-space exploration | Designers can quickly compare different resource budgets. |

## 8. Unsuitable or Problematic Application Examples

| Problematic case | Reason |
|---|---|
| Designs requiring proven optimality | Heuristic schedules do not guarantee global optimum. |
| Very memory-dominated designs | Basic list scheduling may ignore memory-port bottlenecks. |
| Large loop-heavy streaming systems | Loop pipelining or modulo scheduling may be more important. |
| Highly irregular control-flow programs | Branches and dynamic behavior complicate the scheduling model. |
| Safety-critical designs without verification | Scheduling decisions must be validated carefully. |
| Power-critical designs | Priority rules may not directly minimize switching or power. |

## 9. Transfer to Realistic Embedded Systems

List-based scheduling transfers well to embedded systems when the hardware block is:

- computation-oriented,
- resource-constrained,
- mostly static,
- representable as a DFG or CDFG.

It is useful for explaining tradeoffs in FPGA-based embedded accelerators.

It is less sufficient when system behavior is dominated by:

- unpredictable input timing,
- dynamic memory access,
- interrupts,
- communication protocols,
- real-time operating-system scheduling.

In realistic embedded design, list scheduling should be combined with:

- timing analysis,
- memory analysis,
- simulation,
- synthesis reports,
- waveform verification.

## 10. Evaluation Using My Example Implementation

Expression:

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

Critical observation:

```text
parallel ALUs reduce latency, but increase hardware area.
```

This example is suitable because the tradeoff is visible.

However, it is simplified:

- all operations are assumed to be one cycle,
- memory access is ignored,
- routing delay is ignored,
- power is not measured,
- the schedule is very small.

## 11. Risk and Mitigation Table

| Risk | Mitigation | Priority |
|---|---|---|
| Priority rule gives poor schedule | Compare at least two priority rules or justify the selected rule. | High |
| Memory conflicts ignored | Treat memory ports as resources in an extended model. | High |
| Schedule looks correct but RTL differs | Validate using simulation and waveform results. | High |
| Latency improves but area grows too much | Include resource table and area-latency discussion. | Medium |
| Simplified example too small | Clearly state limitations and add transfer discussion. | Medium |
| Timing closure not guaranteed | Mention synthesis/place-and-route timing reports as later validation. | Medium |

## 12. Possible Extensions and Adaptations

- Add memory-port constraints:

```text
number of simultaneous memory accesses <= available memory ports
```

- Compare different priority rules:
  - critical path first,
  - lowest mobility first,
  - highest number of successors first.
- Add multi-cycle operations, for example multiplication taking more than one cycle.
- Add power-aware priority rules.
- Add area-aware scheduling that penalizes extra hardware resources.
- Compare list scheduling with force-directed scheduling.
- Add loop pipelining discussion for repeated computations.
- Use C++ implementation results and VHDL/Questa waveforms as validation evidence.

## 13. Open Research or Discussion Questions

- Which priority rule gives the best schedule for my example and why?
- How does the result change if multiplication takes 2 or 3 cycles?
- How should memory access be included in the scheduling model?
- Can list scheduling be improved using feedback from synthesis reports?
- How can power consumption be considered during scheduling?
- When is list scheduling good enough, and when is force-directed or exact scheduling necessary?
- How much difference exists between the HLS schedule and the final FPGA timing after place-and-route?

## 14. Critical Final Judgment

- List-based scheduling is a strong practical method for HLS because it is simple, fast, and resource-aware.
- It is especially useful for explaining and implementing small to medium resource-constrained hardware examples.
- Its main weakness is that it is heuristic and priority-dependent.
- It should not be presented as an optimal method.
- For realistic embedded systems, it must be combined with memory modeling, synthesis validation, timing checks, and possibly loop-pipelining techniques.

## 15. Milestone 3 Checklist Status

| Requirement | Status |
|---|---|
| Concrete strengths identified | Done |
| Concrete limitations, risks, or hidden assumptions identified | Done |
| Runtime, memory, and scalability considered | Done |
| Applicability to realistic embedded systems evaluated | Done |
| Suitable application case identified | Done |
| Unsuitable or problematic case identified | Done |
| Meaningful extensions and open questions proposed | Done |

## Short Summary

List-based scheduling is useful because it provides a fast and understandable way to create resource-constrained schedules in HLS.  
It is suitable for arithmetic datapaths, FPGA learning examples, and early design-space exploration.  
It is less suitable when optimality, complex memory behavior, loop pipelining, or strict power optimization dominates.  
The final paper should present it as a practical heuristic, not as a globally optimal scheduling method.
