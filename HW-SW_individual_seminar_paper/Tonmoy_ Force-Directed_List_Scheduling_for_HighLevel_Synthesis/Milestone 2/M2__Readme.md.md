# M2 Scientific Contextualization and Tradeoff Analysis: AI Usage Interaction Log
## Metadata
- **Student:** Faysal Ahammed Tonmoy
- **Student ID:** 1230473
- **Course:** HW/SW Codesign / Embedded Electronic Engineering A
- **Topic:** Force-Directed List-Scheduling for High-Level Synthesis


## Milestone purpose
Scientific contextualization and tradeoff analysis. The focus is to position the assigned topic among related approaches, neighboring methods, design alternatives, assumptions, and embedded-system consequences.

## Expected milestone evidence
- related approaches, methods, tools, standards, or paradigms
- competing or neighboring solution strategies
- assumptions behind different approaches
- tradeoffs such as optimality vs feasibility and precision vs runtime
- consequences for embedded systems or constrained hardware
- explanation of sparse or indirect related work

## How to read this file
This Markdown file is a human-readable version of the machine-readable AI usage protocol JSON. It documents only milestone-relevant AI-supported reasoning episodes. It is not a raw chat dump and it is not final paper prose. The important parts are the objective, verification actions, evaluation status, integration decision, and reflection for each interaction.

## Summary of AI usage
### Evaluation status counts
- `accepted`: 8
- `partially-accepted`: 2
- `revised`: 3

### Verification level counts
- `methodological-check`: 5
- `multi-source-validation`: 2
- `primary-source-check`: 2
- `secondary-source-check`: 4

### Usage type counts
- `argument-critique`: 4
- `comparison-structuring`: 4
- `idea-generation`: 1
- `summarization`: 1
- `terminology-clarification`: 2
- `verification-support`: 1

## Interaction overview table
| # | ID | Objective | Verification | Status | Used in work | Section | Usage type |
|---:|---|---|---|---|---|---|---|
| 1 | `e-20103aec` | Contextualize FDLS among HLS scheduling approaches. | `multi-source-validation` | `accepted` | yes | M2 Related approaches overview | `comparison-structuring` |
| 2 | `e-2021ee62` | Explain why related work may be sparse or indirect. | `secondary-source-check` | `accepted` | yes | M2 Explanation of sparse/indirect related work | `argument-critique` |
| 3 | `e-2035433b` | Analyze optimality versus feasibility tradeoff. | `primary-source-check` | `revised` | yes | M2 Optimality vs feasibility | `comparison-structuring` |
| 4 | `e-20417b99` | Compare list scheduling and FDLS under resource constraints. | `methodological-check` | `accepted` | yes | M2 List scheduling vs FDLS | `comparison-structuring` |
| 5 | `e-20580bc8` | Contextualize FDLS against pure Force-Directed Scheduling. | `methodological-check` | `partially-accepted` | yes | M2 Method boundary | `terminology-clarification` |
| 6 | `e-206ddd05` | Analyze precision versus runtime tradeoff in force computation. | `methodological-check` | `accepted` | yes | M2 Precision vs runtime | `argument-critique` |
| 7 | `e-20780a63` | Analyze memory versus accuracy tradeoff. | `methodological-check` | `partially-accepted` | yes | M2 Memory vs computation | `argument-critique` |
| 8 | `e-2089efc0` | Discuss offline versus online scheduling. | `primary-source-check` | `accepted` | yes | M2 Offline vs online computation | `terminology-clarification` |
| 9 | `e-2094adef` | Discuss consequences for constrained hardware. | `multi-source-validation` | `accepted` | yes | M2 Embedded-systems consequences | `idea-generation` |
| 10 | `e-20ac8792` | Compare formal guarantees and practical deployability. | `methodological-check` | `revised` | yes | M2 Guarantees vs deployability | `argument-critique` |
| 11 | `e-20b1e223` | Identify standards/tools/paradigms related to HLS. | `secondary-source-check` | `accepted` | yes | M2 Tools and paradigms | `summarization` |
| 12 | `e-20cb386e` | Build a context comparison table. | `secondary-source-check` | `revised` | yes | M2 Comparison table | `comparison-structuring` |
| 13 | `e-20dda942` | Validate M2 against professor's milestone purpose. | `secondary-source-check` | `accepted` | yes | M2 Final checklist validation | `verification-support` |

## Detailed interaction records
### Interaction 1: e-20103aec
- **Timestamp:** 2026-06-14T12:00:00+02:00
- **Parent ID:** None
- **Objective:** Contextualize FDLS among HLS scheduling approaches.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Compare Force-Directed List-Scheduling with ASAP, ALAP, list scheduling, ILP scheduling, simulated annealing, and genetic algorithm scheduling for HLS.
```

#### AI output summary
The AI created a comparison of exact and heuristic approaches: ASAP/ALAP as bounds, list scheduling as fast greedy scheduling, ILP as exact but expensive, force-directed as balancing resource demand, and metaheuristics as broader design-space exploration.

#### Verification
- **Level:** `multi-source-validation`
- **Actions:**
  - Checked the approach categories against De Micheli's scheduling chapter and Teich/Haubelt's scheduling chapter.
  - Checked the course topic list to ensure these are neighboring seminar topics.
  - Removed unsupported tool-specific claims.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Related approaches overview
- **Usage type:** `comparison-structuring`
- **Direct text reused:** No

#### Reflection
This gave me the scientific landscape: FDLS is not isolated; it sits between simple greedy scheduling and expensive exact/metaheuristic methods.

---

### Interaction 2: e-2021ee62
- **Timestamp:** 2026-06-14T12:10:00+02:00
- **Parent ID:** None
- **Objective:** Explain why related work may be sparse or indirect.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
My professor says related work should not be artificially collected. For FDLS, explain why some related work is direct and some is only neighboring.
```

#### AI output summary
The AI suggested that direct related work is HLS scheduling and force-directed scheduling, while indirect work includes compiler instruction scheduling, real-time task scheduling, and general multiprocessor scheduling because they share scheduling concepts but different objectives and constraints.

#### Verification
- **Level:** `secondary-source-check`
- **Actions:**
  - Checked the professor's milestone checklist for the instruction that weakly related papers should not be artificially collected.
  - Manually classified references into direct HLS scheduling, neighboring scheduling theory, and indirect embedded-systems context.
  - Excluded unrelated operating-system scheduling papers from the core context.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Explanation of sparse/indirect related work
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
This helped me avoid a fake literature review. The milestone should justify why related work is selected, not just list many papers.

---

### Interaction 3: e-2035433b
- **Timestamp:** 2026-06-14T12:20:00+02:00
- **Parent ID:** None
- **Objective:** Analyze optimality versus feasibility tradeoff.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain the tradeoff between ILP-based scheduling and Force-Directed List-Scheduling in terms of optimality, runtime, scalability, and use in HLS.
```

#### AI output summary
The AI argued that ILP can model constraints exactly and may prove optimality for a given formulation, but can scale poorly. FDLS is heuristic, usually faster, and more practical for larger graphs, but does not guarantee global optimum.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Checked the ILP scheduling position in De Micheli and Teich/Haubelt.
  - Verified that heuristic methods are presented separately from exact ILP methods.
  - Changed wording from 'always faster' to 'typically intended to be more scalable' because actual runtime depends on implementation and instance size.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - The AI initially overstated ILP as always impractical. I softened the claim because small ILP instances can be useful and exact.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Optimality vs feasibility
- **Usage type:** `comparison-structuring`
- **Direct text reused:** No

#### Reflection
This is a central tradeoff: exactness is valuable scientifically, but heuristic feasibility is important for engineering.

---

### Interaction 4: e-20417b99
- **Timestamp:** 2026-06-14T12:30:00+02:00
- **Parent ID:** None
- **Objective:** Compare list scheduling and FDLS under resource constraints.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
What are the engineering tradeoffs between ordinary list scheduling and force-directed list scheduling under limited multipliers/adders?
```

#### AI output summary
The AI explained that list scheduling is simple and fast but priority rules may be local. FDLS adds a global-looking resource distribution estimate, which may improve balance but increases computation and still remains heuristic.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that list scheduling and force-directed scheduling are distinct heuristic approaches in the HLS literature.
  - Used a small hand graph to see how fixed priority may create resource peaks.
  - Marked the result as a tradeoff, not a proof of superiority.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 List scheduling vs FDLS
- **Usage type:** `comparison-structuring`
- **Direct text reused:** No

#### Reflection
I can now explain why FDLS was introduced: to guide greedy decisions with a more resource-aware priority.

---

### Interaction 5: e-20580bc8
- **Timestamp:** 2026-06-14T12:40:00+02:00
- **Parent ID:** None
- **Objective:** Contextualize FDLS against pure Force-Directed Scheduling.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Compare pure Force-Directed Scheduling and Force-Directed List-Scheduling. Which one is more constrained by ready-list behavior?
```

#### AI output summary
The AI explained that pure force-directed methods may evaluate possible placements over mobility windows, while list-style scheduling usually works step-by-step with ready operations and resource availability. FDLS therefore combines force-based ranking with list scheduling discipline.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked terminology against De Micheli and Teich/Haubelt to avoid inventing a false standard definition.
  - Kept the explanation as a possible interpretation of FDLS, not a universal formal definition.
  - Noted that the assigned reference must decide the exact variant described in the final paper.

#### Evaluation
- **Status:** `partially-accepted`
- **Usefulness:** `medium`
- **Issues:**
  - The exact term FDLS is less standardized than FDS; the explanation must be cautious.
  - The AI could not prove one universal algorithm structure.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Method boundary
- **Usage type:** `terminology-clarification`
- **Direct text reused:** No

#### Reflection
This made me more careful: I should define exactly what I mean by FDLS in my paper.

---

### Interaction 6: e-206ddd05
- **Timestamp:** 2026-06-14T12:50:00+02:00
- **Parent ID:** None
- **Objective:** Analyze precision versus runtime tradeoff in force computation.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
How does the accuracy of resource-demand estimation trade off against runtime in Force-Directed List-Scheduling?
```

#### AI output summary
The AI explained that frequent recomputation of probability and distribution graphs after each decision may produce more informed priorities but increases runtime. Incremental or approximate updates may be faster but less precise.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that runtime depends on recomputation strategy rather than a fixed formula.
  - Validated the idea with the algorithm steps: ASAP/ALAP windows, probability distributions, scheduling decision, update.
  - Used this as engineering analysis rather than a sourced performance claim.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Precision vs runtime
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
This is a strong engineering tradeoff because the algorithm's quality depends on information that costs time to update.

---

### Interaction 7: e-20780a63
- **Timestamp:** 2026-06-14T13:00:00+02:00
- **Parent ID:** None
- **Objective:** Analyze memory versus accuracy tradeoff.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain possible memory tradeoffs in storing mobility windows, distribution graphs, candidate forces, and ready lists for FDLS.
```

#### AI output summary
The AI listed data structures such as per-operation time windows, per-cycle resource distributions, ready queues, and candidate force tables. It noted that storing more intermediate values can speed selection but increases memory usage.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that these structures logically follow from the algorithm steps.
  - Did not present memory sizes as measured values because no implementation benchmark was used.
  - Kept the discussion at architectural-algorithmic level.

#### Evaluation
- **Status:** `partially-accepted`
- **Usefulness:** `medium`
- **Issues:**
  - The AI's suggested memory estimates were too concrete without implementation. I replaced them with qualitative data-structure discussion.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Memory vs computation
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
Memory is not the main headline of FDLS, but the data structures matter for implementation.

---

### Interaction 8: e-2089efc0
- **Timestamp:** 2026-06-14T13:10:00+02:00
- **Parent ID:** None
- **Objective:** Discuss offline versus online scheduling.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Is Force-Directed List-Scheduling an offline or online scheduling method in HLS? Explain consequences for embedded systems.
```

#### AI output summary
The AI described FDLS as mainly an offline compile-time/architectural synthesis method. The schedule is computed before hardware generation, unlike runtime scheduling in an operating system. This supports deterministic hardware but cannot adapt to arbitrary runtime behavior.

#### Verification
- **Level:** `primary-source-check`
- **Actions:**
  - Checked Teich/Haubelt's distinction between static and dynamic scheduling.
  - Checked the HLS context where scheduling happens before RTL generation.
  - Removed statements about OS scheduling except as contrast.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Offline vs online computation
- **Usage type:** `terminology-clarification`
- **Direct text reused:** No

#### Reflection
This distinction is important for embedded systems: deterministic compile-time scheduling is useful for hardware synthesis, but it is not runtime adaptation.

---

### Interaction 9: e-2094adef
- **Timestamp:** 2026-06-14T13:20:00+02:00
- **Parent ID:** None
- **Objective:** Discuss consequences for constrained hardware.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
What are the consequences of using FDLS for constrained embedded hardware like FPGA datapaths, DSP accelerators, or small ASIC blocks?
```

#### AI output summary
The AI linked FDLS to resource sharing: limiting multipliers/adders can reduce area but may increase latency; balancing resource demand can help choose a feasible schedule before binding and RTL generation.

#### Verification
- **Level:** `multi-source-validation`
- **Actions:**
  - Checked Teich/Haubelt's hardware/software synthesis context and resource constraints.
  - Checked De Micheli's architectural synthesis flow from scheduling to binding.
  - Kept the examples as application contexts, not empirical proof.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Embedded-systems consequences
- **Usage type:** `idea-generation`
- **Direct text reused:** No

#### Reflection
This connected the algorithm to real design consequences: area, latency, and determinism.

---

### Interaction 10: e-20ac8792
- **Timestamp:** 2026-06-14T13:30:00+02:00
- **Parent ID:** None
- **Objective:** Compare formal guarantees and practical deployability.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Explain the tradeoff between formal guarantees and practical deployability for FDLS compared with exact scheduling.
```

#### AI output summary
The AI explained that exact formulations can offer stronger guarantees for a simplified model, while FDLS is easier to deploy as a heuristic but gives no universal optimality guarantee. Both depend on model accuracy.

#### Verification
- **Level:** `methodological-check`
- **Actions:**
  - Checked that ILP's guarantee only applies to the formulated model and constraints.
  - Added the point that a formally optimal schedule can still be poor if the model ignores memory or routing delays.
  - Kept the wording balanced.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - The AI initially made formal methods sound automatically better. I added the limitation that model fidelity matters.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Guarantees vs deployability
- **Usage type:** `argument-critique`
- **Direct text reused:** No

#### Reflection
This became a useful argument for the critical sections later: guarantees and practical usefulness are not the same thing.

---

### Interaction 11: e-20b1e223
- **Timestamp:** 2026-06-14T13:40:00+02:00
- **Parent ID:** None
- **Objective:** Identify standards/tools/paradigms related to HLS.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Which tools, standards, or paradigms can I mention as context for HLS scheduling without drifting away from FDLS?
```

#### AI output summary
The AI suggested mentioning behavioral synthesis/HLS, RTL generation, C/C++/SystemC-based design descriptions, FPGA/ASIC target flows, and scheduling/allocation/binding as core paradigms rather than focusing on vendor tools.

#### Verification
- **Level:** `secondary-source-check`
- **Actions:**
  - Checked Teich/Haubelt's discussion of SystemC and HLS-style design descriptions.
  - Checked De Micheli's architectural synthesis terminology.
  - Did not include vendor-specific tool claims because they were not needed for the milestone.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `medium`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Tools and paradigms
- **Usage type:** `summarization`
- **Direct text reused:** No

#### Reflection
Keeping the context at paradigm level prevents the paper from becoming a tool survey.

---

### Interaction 12: e-20cb386e
- **Timestamp:** 2026-06-14T13:50:00+02:00
- **Parent ID:** None
- **Objective:** Build a context comparison table.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Create a concise comparison table for M2 with methods, idea, strengths, weaknesses, and suitable use cases for HLS scheduling.
```

#### AI output summary
The AI proposed table rows for ASAP/ALAP, list scheduling, FDLS, ILP, simulated annealing, genetic algorithms, and pipelined scheduling. I used the structure but rewrote the content.

#### Verification
- **Level:** `secondary-source-check`
- **Actions:**
  - Checked the table methods against the course topic list and HLS scheduling chapters.
  - Manually revised each strength/weakness so it did not become generic.
  - Did not copy table text directly; used it only as structure.

#### Evaluation
- **Status:** `revised`
- **Usefulness:** `high`
- **Issues:**
  - Some AI table cells were too absolute, e.g., 'best quality'. I replaced them with cautious comparative language.
  - Some methods belonged to broader exploration, not direct competitor categories.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Comparison table
- **Usage type:** `comparison-structuring`
- **Direct text reused:** No

#### Reflection
The table makes the scientific context easier to communicate and prevents a long narrative-only related work section.

---

### Interaction 13: e-20dda942
- **Timestamp:** 2026-06-14T14:00:00+02:00
- **Parent ID:** None
- **Objective:** Validate M2 against professor's milestone purpose.
- **AI tool:** ChatGPT
- **AI model:** GPT-5.5 Thinking

#### Prompt
```text
Check whether my M2 plan satisfies scientific contextualization and tradeoff analysis, not only related-work summary.
```

#### AI output summary
The AI reviewed the planned sections and identified missing items: explanation of indirect related work, explicit tradeoff bullets, and consequences for embedded systems.

#### Verification
- **Level:** `secondary-source-check`
- **Actions:**
  - Checked the professor's M2 checklist: related approaches, alternatives, assumptions, tradeoffs, consequences, and explanation of sparse/indirect related work.
  - Added explicit subsections for tradeoffs and embedded-systems consequences.
  - Confirmed M2 remains structured notes rather than publication-style final prose.

#### Evaluation
- **Status:** `accepted`
- **Usefulness:** `high`
- **Issues:**
  - None documented.

#### Integration
- **Used in submitted work:** Yes
- **Section:** M2 Final checklist validation
- **Usage type:** `verification-support`
- **Direct text reused:** No

#### Reflection
This final review aligned the milestone with the professor's exact intention: contextualization and tradeoffs, not a classical literature list only.

---

## Academic integrity note
The interactions documented here were used as support for understanding, structuring, critique, verification, or communication. The submitted milestone artifact should not reuse AI text directly unless explicitly marked and allowed. Scientific claims must be checked against the assigned references, lecture material, primary sources, implementation output, or manual reasoning before being included in the final work.
