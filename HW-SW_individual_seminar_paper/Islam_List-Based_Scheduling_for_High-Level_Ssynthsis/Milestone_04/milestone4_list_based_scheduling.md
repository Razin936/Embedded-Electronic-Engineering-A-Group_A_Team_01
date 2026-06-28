# Milestone 4: Scientific Communication: Paper and Talk  
## Topic: List-Based Scheduling for High-Level Synthesis

**Student:** Md Azadul Islam  
**Student ID:** 2220070  
**Date:** 28 June 2026

## Purpose

This milestone prepares the final scientific communication for the seminar topic.  
The goal is to synthesize Milestone 1, Milestone 2, and Milestone 3 into a coherent final paper and presentation plan.  
The paper should communicate the technical contribution, scientific context, and critical evaluation.  
The talk should not simply repeat the paper; it should explain the main idea, key tradeoffs, and critical judgment clearly.

## 1. Integration of Previous Milestones

| Previous milestone | Main contribution to final paper and talk |
|---|---|
| Milestone 1 | Technical understanding: problem statement, HLS scheduling idea, formal model, pseudocode, resource constraints, and small example. |
| Milestone 2 | Scientific context: related approaches, design alternatives, assumptions, and tradeoffs. |
| Milestone 3 | Critical evaluation: strengths, limitations, risks, suitable/unsuitable applications, and possible extensions. |
| Milestone 4 | Combines the previous work into a coherent paper structure and presentation structure. |

## 2. Final Paper Communication Goal

- The final paper should explain **what list-based scheduling is**, **why it is useful**, and **where its limitations are**.
- It should not only describe the algorithm.
- It should also show:
  - why scheduling is important in HLS,
  - how data dependencies and resource constraints affect the schedule,
  - how the method compares with related approaches,
  - and when the method is suitable or unsuitable.
- The paper should clearly separate:
  - technical facts,
  - interpretation,
  - implementation observation,
  - critical evaluation.

## 3. Proposed Final Paper Structure

| Section | Title | Purpose |
|---|---|---|
| 1 | Introduction | Introduce HLS, scheduling, resource constraints, and motivation. |
| 2 | Background | Explain DFG/CDFG, ASAP, ALAP, mobility, latency, and resource constraints. |
| 3 | List-Based Scheduling Method | Present ready list, priority rules, pseudocode, and scheduling process. |
| 4 | Example and Implementation Direction | Show the arithmetic example and the 1-ALU vs 2-ALU schedule difference. |
| 5 | Scientific Context and Related Approaches | Compare ASAP/ALAP, force-directed scheduling, ILP/exact methods, loop pipelining, and HLS tools. |
| 6 | Critical Evaluation | Discuss strengths, limitations, hidden assumptions, scalability, and risks. |
| 7 | Embedded-System Relevance | Explain transfer to FPGA/embedded systems, area-latency tradeoff, and validation needs. |
| 8 | Conclusion | Summarize the practical value and limitations of list-based scheduling. |
| 9 | AI Usage Protocol | Transparently document AI-supported reasoning and validation if required by the course. |

## 4. Key Technical Message for the Paper

> List-based scheduling is a practical HLS heuristic that schedules ready operations step by step while respecting data dependencies and limited hardware resources. It is simple, scalable, and useful for resource-constrained designs, but it does not guarantee a globally optimal schedule and depends strongly on the chosen priority rule.

## 5. Main Technical Explanation to Include

### Data-flow graph

```text
G = (V, E)
```

- `V`: operations.
- `E`: dependency edges.
- `(u, v) in E`: operation `v` depends on operation `u`.

### Precedence constraint

```text
t(v) >= t(u) + l(u)
```

This ensures that an operation can start only after its predecessors have finished.

### Resource constraint

```text
number of active operations of resource type r at cycle c <= R_r
```

This ensures that the number of active operations of one resource type does not exceed the available hardware units.

### Mobility

```text
mobility(v) = ALAP(v) - ASAP(v)
```

Low mobility means the operation is urgent.  
High mobility means the operation has more scheduling freedom.

## 6. Example to Use in Paper and Talk

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

### Schedule comparison

| Resource setting | Schedule idea | Latency |
|---|---|---|
| 1 ALU + 1 multiplier | Additions must be separated because only one ALU is available. | 4 cycles |
| 2 ALUs + 1 multiplier | Two additions can execute in parallel. | 3 cycles |

Main interpretation:

```text
more hardware resources → lower latency but higher area/cost
```

## 7. Facts, Interpretation, and Critique

| Category | Example for this topic |
|---|---|
| Fact | List scheduling uses a ready list and schedules operations under dependency and resource constraints. |
| Interpretation | It is useful because it gives a practical schedule without solving a full global optimization problem. |
| Implementation observation | In the small example, using two ALUs can reduce the schedule from 4 cycles to 3 cycles. |
| Critique | The method is heuristic and depends strongly on the priority rule, so it should not be presented as globally optimal. |
| Limitation | Simple examples often ignore memory ports, routing delay, power, branches, and loops. |

## 8. Presentation Goal

- The presentation should explain the core idea visually and simply.
- It should not be a text-heavy copy of the paper.
- The talk should focus on:
  - motivation,
  - data-flow graph intuition,
  - ready list concept,
  - 1-ALU vs 2-ALU example,
  - main tradeoffs,
  - strengths and limitations,
  - final critical judgment.

## 9. Proposed Slide Structure

| Slide | Title | Main message |
|---|---|---|
| 1 | Title | Topic, student name, student ID, seminar context. |
| 2 | Motivation | HLS needs scheduling because operations have dependencies and resources are limited. |
| 3 | HLS Flow | Show algorithm to DFG/CDFG to scheduling to RTL. |
| 4 | Scheduling Problem | Explain dependencies, resources, and latency. |
| 5 | Data-Flow Graph Example | Show the expression `z=(a+b)*(c+d)+e`. |
| 6 | List-Based Scheduling Concept | Explain ready list, priorities, and resource checks. |
| 7 | Pseudocode / Algorithm Steps | Show simplified algorithm in 5–7 steps. |
| 8 | 1 ALU Schedule | Show why the two additions must be separated. |
| 9 | 2 ALU Schedule | Show how parallel ALUs reduce latency. |
| 10 | Tradeoff | More resources reduce latency but increase area and power. |
| 11 | Related Approaches | Compare ASAP/ALAP, force-directed scheduling, ILP, and loop pipelining. |
| 12 | Strengths and Limitations | Present practical advantages and main risks. |
| 13 | Embedded-System Relevance | Explain why this matters for FPGA/embedded accelerators. |
| 14 | Conclusion | Practical heuristic, useful but not globally optimal. |
| 15 | Discussion Questions | Ask about priority rules, memory constraints, and timing validation. |

## 10. Suggested Visuals and Diagrams

- HLS flow diagram:

```text
Algorithm → DFG → Schedule → RTL
```

- Data-flow graph for:

```text
z = (a+b) * (c+d) + e
```

- Table comparing 1 ALU and 2 ALUs.
- Simple timeline diagram:

```text
Cycle 1 → Cycle 2 → Cycle 3 → Cycle 4
```

- Tradeoff diagram:

```text
Area ↑  →  Latency ↓
```

- Optional waveform screenshot from VHDL/Questa implementation.

## 11. Talk Opening

A possible natural opening:

> Today I will present list-based scheduling for High-Level Synthesis. In HLS, we start from a high-level algorithm and generate hardware. One important question is: in which clock cycle should each operation run? This is not only a software-style ordering problem, because hardware resources such as adders and multipliers are limited. List-based scheduling is a practical method to create a feasible schedule under these constraints.

## 12. Talk Closing

A possible conclusion:

> To conclude, list-based scheduling is useful because it is simple, fast, and resource-aware. It helps designers understand the tradeoff between latency and hardware cost. However, it is a heuristic, so the final result depends on the priority rule and it does not guarantee a globally optimal schedule. For realistic HLS design, it should be combined with memory modeling, simulation, synthesis reports, and timing validation.

## 13. Discussion Questions for the Presentation

- Which priority rule should be preferred for a small arithmetic graph: critical path, mobility, or number of successors?
- How would the schedule change if multiplication takes more than one cycle?
- Should memory accesses be treated as resources like ALUs and multipliers?
- When is list scheduling good enough, and when is an exact method or force-directed scheduling better?
- How much can the final FPGA timing differ from the schedule created during HLS?

## 14. AI Usage and Transparency

- AI support was used to structure milestone content, compare approaches, and improve the clarity of explanation.
- The AI output was not used as an unchecked final authority.
- The content should be verified against the assigned HLS references, lecture material, and implementation results.
- The AI usage protocol should document:
  - the objective of each AI-supported interaction,
  - the prompt,
  - the output summary,
  - verification actions,
  - evaluation status,
  - issues or limitations,
  - reflection.

## 15. Milestone 4 Checklist Status

| Requirement | Status |
|---|---|
| Paper integrates Milestone 1–3 instead of being generated independently | Done |
| Technical explanation is understandable without oversimplifying | Done |
| Facts, interpretation, critique, and speculation are separated | Done |
| Slides focus on explanation and discussion, not text-heavy reproduction | Done |
| Presentation includes examples, diagrams, or formal sketches | Done |
| Talk addresses strengths, limitations, and application consequences | Done |
| AI usage is documented according to the required protocol | Done |

## Short Summary

Milestone 4 prepares the final communication of the topic.  
The final paper should combine technical understanding, scientific context, and critical evaluation.  
The presentation should explain the main scheduling idea visually and clearly, using the small arithmetic example and the latency-area tradeoff.  
The main message is that list-based scheduling is a practical HLS heuristic: useful, fast, and understandable, but not globally optimal.
