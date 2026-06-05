# References and Reading Notes for Milestone 3

**Student:** Md Jubair Salehin Razin  
**Student ID:** 1230473  
**Topic:** Sporadic Server  
**Milestone:** Milestone 3: Critical Evaluation and Transfer  

---

## 1. Main References

### [1] Sprunt, Sha, and Lehoczky — Original Sporadic Server Paper

**Reference**

Sprunt, B., Sha, L., and Lehoczky, J. P.  
*Aperiodic Task Scheduling for Hard-Real-Time Systems.*  
Real-Time Systems, 1, 27–60, 1989.  
DOI: 10.1007/BF02341920

**Why it is relevant**

This is the primary technical source for the Sporadic Server. It introduces the approach for serving aperiodic tasks in hard real-time systems while preserving schedulability of periodic tasks.

**How it is related to Milestone 3**

This source is important for evaluating:

- original motivation of the Sporadic Server
- aperiodic task handling
- budget and replenishment concept
- protection of periodic hard real-time tasks
- comparison with simpler aperiodic service methods

**Use in report**

Use this source when explaining the core technical idea and when evaluating whether the approach gives a strong balance between responsiveness and schedulability.

---

### [2] Liu and Layland — Fixed-Priority Real-Time Scheduling Background

**Reference**

Liu, C. L., and Layland, J. W.  
*Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment.*  
Journal of the ACM, 20(1), 46–61, 1973.  
DOI: 10.1145/321738.321743

**Why it is relevant**

This is a classical reference for hard real-time scheduling. It provides background for periodic task scheduling and fixed-priority reasoning.

**How it is related to Milestone 3**

The Sporadic Server is evaluated in the context of hard real-time scheduling. To judge its usefulness, it is necessary to understand why periodic task schedulability and priority-based analysis matter.

**Use in report**

Use this source when discussing:

- periodic hard real-time tasks
- schedulability assumptions
- fixed-priority scheduling context
- why uncontrolled aperiodic execution can be dangerous

---

### [3] Buttazzo — Real-Time Systems Textbook

**Reference**

Buttazzo, G. C.  
*Hard Real-Time Computing Systems: Predictable Scheduling Algorithms and Applications.*  
3rd edition, Springer, 2011.  
DOI: 10.1007/978-1-4614-0676-1

**Why it is relevant**

This textbook gives broader context for predictable scheduling algorithms, server-based scheduling, and real-time systems design.

**How it is related to Milestone 3**

It is useful for understanding the practical meaning of:

- hard real-time guarantees
- server mechanisms
- schedulability analysis
- implementation overhead
- predictable embedded-system behavior

**Use in report**

Use this source for textbook-level support when discussing limitations, assumptions, runtime overhead, and implementation implications.

**Important honesty note**

Only cite specific details from this book if the exact section was actually read and verified. If access is not available, do not pretend that it was used directly for detailed claims.

---

## 2. Reference Notes for BibTeX Management

The professor requested short notes about how each reference is related to the seminar topic. These notes can be stored in Zotero/Citavi notes or kept in this Markdown file.

| Reference | Relation to Topic | Used For |
|---|---|---|
| Sprunt, Sha, and Lehoczky | Primary source for Sporadic Server | Core mechanism, strengths, limitations |
| Liu and Layland | Classical hard real-time scheduling background | Fixed-priority context and schedulability assumptions |
| Buttazzo | Textbook context for real-time scheduling | Broader explanation of server-based scheduling and implementation implications |

---

## 3. Suggested BibTeX Entries

```bibtex
@article{sprunt1989aperiodic,
  author  = {Sprunt, Brinkley and Sha, Lui and Lehoczky, John P.},
  title   = {Aperiodic Task Scheduling for Hard-Real-Time Systems},
  journal = {Real-Time Systems},
  volume  = {1},
  pages   = {27--60},
  year    = {1989},
  doi     = {10.1007/BF02341920}
}

@article{liu1973scheduling,
  author  = {Liu, C. L. and Layland, James W.},
  title   = {Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment},
  journal = {Journal of the ACM},
  volume  = {20},
  number  = {1},
  pages   = {46--61},
  year    = {1973},
  doi     = {10.1145/321738.321743}
}

@book{buttazzo2011hard,
  author    = {Buttazzo, Giorgio C.},
  title     = {Hard Real-Time Computing Systems: Predictable Scheduling Algorithms and Applications},
  edition   = {3},
  publisher = {Springer},
  year      = {2011},
  doi       = {10.1007/978-1-4614-0676-1}
}
```

---

## 4. How These References Support Milestone 3

### Strengths of Sporadic Server

Supported mainly by Sprunt, Sha, and Lehoczky. The original paper explains why aperiodic tasks need better service than background execution while maintaining hard real-time guarantees.

### Limitations and Risks

Supported by the practical interpretation of the algorithm. The need for budget tracking, replenishment handling, and accurate timing follows from the server mechanism itself.

### Assumptions

Supported by hard real-time scheduling theory. The analysis depends on known task parameters, bounded execution times, and predictable scheduling behavior.

### Runtime and Memory Implications

Supported by the implementation requirements of the server mechanism. Budget accounting, replenishment queues, and aperiodic request queues create additional runtime and memory overhead.

### Suitable Applications

Supported by the general workload pattern of systems with periodic hard real-time tasks and bounded irregular events, such as automotive, industrial control, robotics, and communication gateways.

### Unsuitable Applications

Derived from cases where the assumptions do not hold, such as continuous aperiodic overload, very small microcontrollers, or systems with unknown timing behavior.

---

## 5. AI Usage Protocol Note

The AI usage for Milestone 3 is documented separately in:

```text
ai_usage_protocol_1230473_Milestone_3.json
```

This references file is not an AI protocol file. It is a reading and citation support file for GitHub organization.

The AI output should not be cited as a technical source. Technical claims must be checked against academic sources and rewritten in the student's own words.
