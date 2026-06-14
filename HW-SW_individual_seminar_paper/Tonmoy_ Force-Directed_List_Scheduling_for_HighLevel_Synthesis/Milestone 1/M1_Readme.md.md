# M1 Technical Understanding and Research Framing: AI Usage Interaction Log
## Metadata
- **Student:** Faysal Ahammed Tonmoy
- **Student ID:** 1230473
- **Course:** HW/SW Codesign / Embedded Electronic Engineering A
- **Topic:** Force-Directed List-Scheduling for High-Level Synthesis
- **Protocol version:** 0.2


## Milestone purpose
Technical understanding and research framing before polished prose. The focus is to show that the technical contribution, assumptions, problem setting, algorithmic model, and open questions were understood.

## Expected milestone evidence
- problem statement and motivation
- core technical idea
- relevant formalism, algorithm, architecture, or model
- assumptions and scope limitations
- runtime, memory, scalability, or resource aspects
- unclear points and questions for feedback

## How to read this file
This Markdown file is a human-readable version of the machine-readable AI usage protocol JSON. It documents only milestone-relevant AI-supported reasoning episodes. It is not a raw chat dump and it is not final paper prose. The important parts are the objective, verification actions, evaluation status, integration decision, and reflection for each interaction.

## Summary of AI usage
### Evaluation status counts
- `accepted`: 5
- `partially-accepted`: 3
- `revised`: 3

### Verification level counts
- `empirical-check`: 1
- `methodological-check`: 3
- `multi-source-validation`: 1
- `plausibility-check`: 1
- `primary-source-check`: 4
- `secondary-source-check`: 1

### Usage type counts
- `argument-critique`: 2
- `comparison-structuring`: 1
- `idea-generation`: 2
- `modeling-support`: 1
- `summarization`: 2
- `terminology-clarification`: 2
- `verification-support`: 1

## Interaction overview table
| # | ID | Objective | Verification | Status | Used in work | Section | Usage type |
|---:|---|---|---|---|---|---|---|
| 1 | `e-101d62d7` | Identify the technical problem setting of Force-Directed List-Scheduling in High-Level Synthesis. | `primary-source-check` | `revised` | yes | M1 Problem statement and motivation | `terminology-clarification` |
| 2 | `e-1029c3cd` | Clarify the relation between HLS, architectural synthesis, RTL, datapath, and control unit. | `primary-source-check` | `accepted` | yes | M1 Core technical idea | `summarization` |
| 3 | `e-103ce4eb` | Identify the formal model used for scheduling. | `methodological-check` | `partially-accepted` | yes | M1 Formalism / model | `modeling-support` |
| 4 | `e-104a5689` | Understand ASAP, ALAP, mobility, and probability distribution before force calculation. | `methodological-check` | `revised` | yes | M1 Relevant algorithmic basics | `terminology-clarification` |
| 5 | `e-1050364e` | Understand the force computation idea without copying source text. | `primary-source-check` | `partially-accepted` | yes | M1 Core technical idea | `summarization` |
| 6 | `e-106eef89` | Clarify how list scheduling is combined with force-directed priority. | `multi-source-validation` | `revised` | yes | M1 Algorithm positioning | `comparison-structuring` |
| 7 | `e-10750b53` | Identify assumptions and scope limitations. | `primary-source-check` | `accepted` | yes | M1 Assumptions and scope limitations | `argument-critique` |
| 8 | `e-1086d425` | Analyze runtime, memory, and scalability at a first-milestone level. | `methodological-check` | `accepted` | yes | M1 Runtime, memory, and scalability | `argument-critique` |
| 9 | `e-109aa0d4` | Prepare a small application example for the technical briefing. | `empirical-check` | `partially-accepted` | yes | M1 Example / intuition | `idea-generation` |
| 10 | `e-10a0c26e` | Document unclear points for professor feedback. | `plausibility-check` | `accepted` | yes | M1 Open points and questions | `idea-generation` |
| 11 | `e-10b5f9bd` | Check if the M1 content satisfies the professor's checklist. | `secondary-source-check` | `accepted` | yes | M1 Final checklist validation | `verification-support` |

## Detailed interaction records
### Interaction 1: e-101d62d7
- **Timestamp:** 2026-06-14T10:00:00+02:00
- **Parent ID:** None
- **Objective:** Identify the technical problem setting of Force-Directed List-Scheduling in High-Level Synthesis.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain from scratch what problem Force-Directed List-Scheduling solves in High-Level Synthesis, but keep the answer as technical notes, not final paper prose.
```

#### AI output summary
The AI framed the problem as scheduling operations from a data-flow or control-data-flow graph into clock steps while respecting precedence and limited resources. It emphasized that scheduling influences latency, resource usage, and later binding.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Compared the explanation with De Micheli's scheduling chapter where ASAP, ALAP, list scheduling, force-directed scheduling, and resource-constrained scheduling are organized as architectural synthesis topics.
  - Checked against Teich/Haubelt that scheduling is one of the fundamental synthesis tasks together with allocation and binding.
  - Removed generic claims about modern HLS tools unless they were directly needed.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - The first answer overused the phrase 'optimal'; I corrected it to 'heuristic' because force-directed and list-based variants do not generally guarantee global optimality.
  - Needed manual clarification between pure Force-Directed Scheduling and Force-Directed List-Scheduling.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Problem statement and motivation
- **Usage type:** `terminology-clarification`
- **Direct text reused:** No

#### Reflection
I learned that the milestone must start from the scheduling problem itself, not from a polished abstract. The central framing is resource-aware scheduling in HLS, not generic task scheduling.

---

### Interaction 2: e-1029c3cd
- **Timestamp:** 2026-06-14T10:09:00+02:00
- **Parent ID:** None
- **Objective:** Clarify the relation between HLS, architectural synthesis, RTL, datapath, and control unit.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Give me a short technical explanation of how a behavioral description becomes RTL in HLS, and where scheduling fits between C/SystemC and RTL.
```

#### AI output summary
The AI described HLS as converting behavioral algorithms into a datapath/control-path architecture. Scheduling assigns operations to cycles; allocation chooses resource types and counts; binding maps operations and variables to hardware units and registers.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Verified the three synthesis tasks against Teich/Haubelt's explanation of allocation, scheduling, and binding.
  - Checked De Micheli's architectural synthesis contents on temporal scheduling and spatial binding.
  - Kept the explanation as notes for the milestone, not as final prose.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Core technical idea
- **Usage type:** `summarization`
- **Direct text reused:** No

#### Reflection
This helped me place FDLS inside the complete HLS flow: behavior -> scheduled operations -> bound resources -> RTL datapath/control.

---

### Interaction 3: e-103ce4eb
- **Timestamp:** 2026-06-14T10:18:00+02:00
- **Parent ID:** None
- **Objective:** Identify the formal model used for scheduling.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
What formal objects do I need for a technical briefing on Force-Directed List-Scheduling: graph, operations, latency, mobility, resources, and schedule variables?
```

#### AI output summary
The AI listed a directed acyclic data-flow graph G=(V,E), operation type function type(v), duration or unit-delay assumption, ASAP/ALAP times, mobility interval, resource limits, and a schedule function s:V -> time step.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that data-flow graphs and control-data-flow graphs are valid modeling bases in Teich/Haubelt.
  - Checked that De Micheli treats scheduling as assigning operations to time steps under precedence and resource constraints.
  - Manually removed any unnecessary advanced mathematical notation that would make the 2-4 page briefing unclear.

#### Evaluation
- **Status:** `partially-accepted`
- **Usefulness:** `high`
- **Issues:**
  - The AI initially assumed all operations have unit latency. I kept this only as a simplifying assumption and noted that multi-cycle operations are possible.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Formalism / model
- **Usage type:** `modeling-support`
- **Direct text reused:** No

#### Reflection
The key formalism is simple enough for the first milestone: operations as graph nodes, dependencies as edges, time steps as schedule slots, and resources as constraints.

---

### Interaction 4: e-104a5689
- **Timestamp:** 2026-06-14T10:28:00+02:00
- **Parent ID:** None
- **Objective:** Understand ASAP, ALAP, mobility, and probability distribution before force calculation.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain ASAP, ALAP, mobility, and operation probability distribution for Force-Directed Scheduling using a small HLS scheduling example.
```

#### AI output summary
The AI explained ASAP as earliest feasible cycle, ALAP as latest feasible cycle under a latency bound, mobility as the interval between them, and probability as 1/mobility-window-size over possible cycles before fixing an operation.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Validated the logic by manually computing a small example with three dependent operations.
  - Checked that ASAP/ALAP are prerequisite scheduling concepts in both Teich/Haubelt and De Micheli.
  - Corrected notation so mobility includes all feasible integer time steps, not only the difference ALAP-ASAP.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - The first explanation confused mobility length with mobility value; I corrected it to a set/window of feasible cycles.
  - Needed manual example to avoid accepting the formula blindly.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Relevant algorithmic basics
- **Usage type:** `terminology-clarification`
- **Direct text reused:** No

#### Reflection
This became one of the main technical checkpoints. Without ASAP/ALAP and mobility, force computation is impossible to explain.

---

### Interaction 5: e-1050364e
- **Timestamp:** 2026-06-14T10:39:00+02:00
- **Parent ID:** None
- **Objective:** Understand the force computation idea without copying source text.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain force computation in Force-Directed Scheduling in my own words: distribution graph, self-force, predecessor/successor force, and total force.
```

#### AI output summary
The AI explained that the distribution graph estimates expected resource demand per time step. Scheduling one operation changes this distribution; the force measures the change. A low or negative total force is preferred because it tends to balance resource usage and reduce peaks.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Checked the force terminology against the classic force-directed scheduling description from Paulin and Knight as cited in HLS literature.
  - Checked De Micheli's treatment of force-directed scheduling as a heuristic scheduling algorithm.
  - Manually rewrote the concept using my own wording and did not reuse the AI's sentences.

#### Evaluation
- **Status:** `partially-accepted`
- **Usefulness:** `high`
- **Issues:**
  - The AI gave a simplified formula but did not fully prove it. I kept the intuition and marked the exact force equations as something to verify further.
  - The AI did not clearly separate self-force from predecessor/successor force in the first attempt.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Core technical idea
- **Usage type:** `summarization`
- **Direct text reused:** No

#### Reflection
I understood the central idea: schedule decisions are scored by how much they change expected resource pressure across cycles.

---

### Interaction 6: e-106eef89
- **Timestamp:** 2026-06-14T10:49:00+02:00
- **Parent ID:** None
- **Objective:** Clarify how list scheduling is combined with force-directed priority.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
What makes Force-Directed List-Scheduling different from ordinary list scheduling and from pure force-directed scheduling?
```

#### AI output summary
The AI explained that ordinary list scheduling chooses ready operations by priority under resource limits, while force-directed scheduling evaluates candidate time placements using force. FDLS can use force as a priority metric inside a ready-list or list-scheduling framework.

#### Verification
- **Level:** `multi-source-validation`
- **Actions:**
  - Checked De Micheli's separate presentation of list scheduling and force-directed scheduling.
  - Checked Teich/Haubelt's list scheduling and force-directed scheduling sections to avoid merging them incorrectly.
  - Marked the term 'Force-Directed List-Scheduling' as a hybrid/variant needing careful wording in the milestone.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - The AI initially made FDLS sound like a universally standardized single algorithm. I corrected the wording to 'hybrid scheduling strategy' because implementations may differ.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Algorithm positioning
- **Usage type:** `comparison-structuring`
- **Direct text reused:** No

#### Reflection
This clarification is important for my exact topic. The paper must not accidentally discuss only pure FDS or only list scheduling.

---

### Interaction 7: e-10750b53
- **Timestamp:** 2026-06-14T11:01:00+02:00
- **Parent ID:** None
- **Objective:** Identify assumptions and scope limitations.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
List the important assumptions behind Force-Directed List-Scheduling for HLS and explain which assumptions may be limitations.
```

#### AI output summary
The AI listed assumptions such as known operation graph, static scheduling, known resource types, finite latency window, deterministic operation delays, and approximate resource probability. It also identified limitations for dynamic control flow, memory bottlenecks, and large design spaces.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Compared assumptions with the scheduling models in De Micheli and Teich/Haubelt.
  - Removed speculative statements about commercial tools that were not verified.
  - Kept only assumptions directly connected to HLS scheduling.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Assumptions and scope limitations
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
The assumptions are not just details; they define when the algorithm can be explained cleanly and when it may fail.

---

### Interaction 8: e-1086d425
- **Timestamp:** 2026-06-14T11:12:00+02:00
- **Parent ID:** None
- **Objective:** Analyze runtime, memory, and scalability at a first-milestone level.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Give me a careful first-level runtime and memory discussion for Force-Directed List-Scheduling without pretending exact complexity if implementation details vary.
```

#### AI output summary
The AI suggested describing cost drivers rather than a single universal complexity: number of operations, number of cycles in mobility windows, number of resource types, recomputation of distribution graphs, and updates after each scheduled operation.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that the professor's milestone asks for runtime, memory, scalability, or resource aspects where applicable.
  - Kept the discussion qualitative because exact complexity depends on data structures and update strategy.
  - Verified that FDLS is a heuristic, so scalability is a key evaluation dimension.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Runtime, memory, and scalability
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
I learned to write complexity as implementation-dependent cost drivers instead of inventing a fake precise Big-O formula.

---

### Interaction 9: e-109aa0d4
- **Timestamp:** 2026-06-14T11:23:00+02:00
- **Parent ID:** None
- **Objective:** Prepare a small application example for the technical briefing.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Create a small HLS example with additions and multiplications that can demonstrate operation scheduling, resource constraints, and FDLS intuition.
```

#### AI output summary
The AI proposed a small arithmetic expression/data-flow graph with multiplications and additions, showing that parallel operations may compete for limited multipliers or adders. This can be used to illustrate why force-based priority tries to avoid resource peaks.

#### Verification
- **Level:** `empirical-check`
- **Actions:**
  - Drew the example graph manually and checked precedence constraints.
  - Confirmed that the example can be scheduled by hand without needing a full HLS tool.
  - Adjusted the example so it fits a short milestone briefing instead of becoming a full implementation section.

#### Evaluation
- **Status:** `partially-accepted`
- **Usefulness:** `medium`
- **Issues:**
  - The AI's first example had too many nodes for a short briefing. I reduced it to a small expression graph.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Example / intuition
- **Usage type:** `idea-generation`
- **Direct text reused:** No

#### Reflection
A small graph is useful because it forces me to understand the algorithm rather than only define it.

---

### Interaction 10: e-10a0c26e
- **Timestamp:** 2026-06-14T11:33:00+02:00
- **Parent ID:** None
- **Objective:** Document unclear points for professor feedback.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
What unclear points and feedback questions should I document for a first milestone on Force-Directed List-Scheduling?
```

#### AI output summary
The AI suggested questions about exact definition of FDLS in the assigned reference, treatment of multi-cycle operations, memory operations, whether C++ implementation should implement full FDS or only the force computation, and how much RTL context is expected.

#### Verification
- **Level:** `plausibility-check`
- **Actions:**
  - Compared the suggested questions with my actual uncertainties.
  - Removed questions that were too broad or not relevant to the seminar topic.
  - Kept questions that help clarify professor expectations.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `medium`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Open points and questions
- **Usage type:** `idea-generation`
- **Direct text reused:** No

#### Reflection
This interaction helped me make uncertainty visible instead of hiding it. That matches the milestone purpose.

---

### Interaction 11: e-10b5f9bd
- **Timestamp:** 2026-06-14T11:44:00+02:00
- **Parent ID:** None
- **Objective:** Check if the M1 content satisfies the professor's checklist.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Review my planned Milestone 1 structure against the professor's checklist: problem, contribution, formalism, assumptions, runtime/memory/scalability, and open questions.
```

#### AI output summary
The AI mapped the planned sections to the checklist and pointed out that I should avoid publication-style prose. It recommended bullet points, formulas, pseudocode fragments, and concise technical notes.

#### Verification
- **Level:** `secondary-source-check`
- **Actions:**
  - Checked against the provided milestone checklist notebook that M1 should be a short structured technical briefing and not polished final prose.
  - Verified that M1-M3 should emphasize reasoning, analysis, and structured notes.
  - Ensured all planned sections correspond to professor's expected output.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M1 Final checklist validation
- **Usage type:** `verification-support`
- **Direct text reused:** No

#### Reflection
This final check made the M1 artifact closer to the professor's expected milestone style.

---

## Academic integrity note
The interactions documented here were used as support for understanding, structuring, critique, verification, or communication. The submitted milestone artifact should not reuse AI text directly unless explicitly marked and allowed. Scientific claims must be checked against the assigned references, lecture material, primary sources, implementation output, or manual reasoning before being included in the final work.
