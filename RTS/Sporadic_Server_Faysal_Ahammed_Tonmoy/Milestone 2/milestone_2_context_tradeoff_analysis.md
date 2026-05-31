# Milestone 2: Scientific Contextualization and Tradeoff Analysis

**Topic:** Sporadic Server
**Name:** Faysal Ahammed Tonmoy
**Matriculation No.:** 1230473
**Team:** A1
**Course:** Real-Time Systems

---

## 1. Purpose of this Milestone

Milestone 1 was about understanding the Sporadic Server itself: what it is, how its budget works, and how the replenishment mechanism helps to serve irregular jobs. Milestone 2 does not repeat only that basic explanation. Instead, it places the Sporadic Server in a wider scientific and engineering context.

The goal of this milestone is to understand which other scheduling approaches deal with the same problem, what assumptions they rely on, and what tradeoffs follow from choosing one method instead of another. In simple words, Milestone 1 answered what a Sporadic Server is. Milestone 2 looks at why it was proposed, what the alternatives are, and what the choice means for a real embedded or real-time system.

---

## 2. Position of Sporadic Server in Real-Time Scheduling

The Sporadic Server is a method for handling aperiodic and sporadic tasks inside a fixed-priority, preemptive real-time system. In many explanations, the periodic tasks are assumed to be scheduled by Rate Monotonic priorities. The server is used to reserve a fixed amount of processor time for jobs that do not arrive regularly.

It is important to say what the Sporadic Server is not. It is not a complete scheduling policy and it is not a replacement for Rate Monotonic or EDF. It works together with a base scheduling policy. Its purpose is narrower: it gives irregular jobs bounded and prioritized processor time so that they can get a better response time, while the periodic real-time tasks are still protected.

The original Sporadic Server mechanism was introduced by Sprunt, Sha, and Lehoczky. Their work is important because it addresses the problem of improving aperiodic responsiveness without losing the ability to reason about schedulability.

---

## 3. Related and Neighboring Approaches

The approaches in this section are included because they deal with the same main problem: serving aperiodic work in a system that also contains periodic real-time tasks. I do not include weakly related papers only to make the reference list longer.

### 3.1 Background Scheduling

Background scheduling is the simplest option. Aperiodic jobs run at the lowest priority, which means they run only when no periodic task is ready. This protects the periodic tasks very well because aperiodic jobs do not interrupt them.

The disadvantage is response time. If the periodic workload is high, there may be very little idle time. Then an aperiodic job can wait for a long time. Compared with this, the Sporadic Server gives aperiodic work a dedicated budget at a chosen priority, instead of leaving it only to idle processor time.

### 3.2 Polling Server

A Polling Server is a periodic task that checks whether aperiodic jobs are waiting. At each activation, it serves pending aperiodic jobs up to its capacity. If no aperiodic job is waiting at that time, the capacity for that period is lost.

This method is simple and fits well into normal periodic schedulability analysis because the server behaves like a periodic task. The weakness is that a job arriving just after the polling instant may wait almost a full server period. Capacity can also be wasted when the server is activated but has nothing to do.

From my current understanding, the Sporadic Server tries to improve this by avoiding unnecessary loss of useful capacity while still keeping interference controlled through its replenishment rule.

### 3.3 Deferrable Server

A Deferrable Server is also based on a server budget, but it keeps unused capacity during the current period. This means an aperiodic job that arrives later in the period can still be served quickly if budget remains.

This gives better response time than the Polling Server. However, the preserved capacity can create more interference for lower-priority periodic tasks. For example, the server may execute late in one period and then again early in the next period after replenishment. This can create a stronger burst of execution.

The Sporadic Server is closely related to the Deferrable Server because both try to improve aperiodic response time. The difference is that the Sporadic Server uses a more careful replenishment rule to control when consumed budget becomes available again.

### 3.4 Priority Exchange and Slack Stealing

Priority Exchange is another related fixed-priority approach. It tries to preserve unused high-priority capacity by exchanging it down to lower priority levels. This can use capacity better than a simple Polling Server, but it also needs more bookkeeping.

Slack Stealing is different. It tries to calculate how much the periodic tasks can be delayed without missing deadlines. This available slack can then be used to execute aperiodic jobs. If the slack is computed correctly, this can give very good response times. The disadvantage is that slack calculation can be complex, especially if it must be done at runtime.

Both Priority Exchange and Slack Stealing are related to Sporadic Server because they also try to improve aperiodic response time while protecting periodic tasks. However, they are not the main focus of my topic.

### 3.5 EDF-based Servers

The methods above mainly belong to fixed-priority scheduling. In EDF-based scheduling, there are also server methods, such as the Total Bandwidth Server and the Constant Bandwidth Server. These methods reserve processor bandwidth for aperiodic or sporadic work under dynamic-priority scheduling.

They are related because they solve a similar problem, but they belong to a different scheduling paradigm. Therefore, I treat EDF-based servers as indirect context rather than as the main comparison.

---

## 4. Main Tradeoffs

### 4.1 Response Time vs Schedulability

The first tradeoff is between fast aperiodic response time and protection of periodic real-time tasks.

Background scheduling protects periodic tasks very well, but aperiodic jobs may wait too long. Polling Server is simple and easier to analyse, but response time can be poor if jobs arrive just after polling. Deferrable Server improves response time, but it may increase interference. The Sporadic Server tries to balance both sides by giving aperiodic jobs budget while controlling how consumed budget is replenished.

This does not mean that Sporadic Server is always the best choice. Depending on the system parameters and job arrival pattern, another method may sometimes give better response time or be easier to implement. The main strength of Sporadic Server is that it tries to combine good responsiveness with controlled interference.

### 4.2 Simplicity vs Implementation Complexity

Polling Server is the simplest among these server methods because it mainly needs periodic activation and capacity checking. Deferrable Server is slightly more complex because unused capacity is preserved during the period.

Sporadic Server is more complex because the system must track how much budget was used and when that budget should be replenished. There may be multiple pending replenishments. This means more state, more logic, and more risk of implementation errors.

So the tradeoff is clear: more control over timing gives better behaviour, but it also increases implementation complexity.

### 4.3 Runtime and Memory Overhead

Sporadic Server needs extra runtime work. The scheduler must check whether budget is available, update the remaining budget while the server runs, manage a queue of aperiodic jobs, and manage future replenishment events.

There is also memory overhead because the system must store information about the server budget, waiting jobs, and scheduled replenishments. Timer or interrupt events are also needed when replenishment times arrive.

For a powerful computer this overhead may be small, but in embedded systems CPU time and memory are limited. Therefore, the overhead is part of the design decision.

### 4.4 Formal Guarantees vs Practical Implementation

Theoretical guarantees depend on assumptions. These include accurate worst-case execution times, correct priority assignment, ideal preemption, exact timing, and correct implementation of the replenishment rule.

Real systems are not perfect. Timer resolution is limited, interrupt latency exists, scheduler overhead is not zero, and measured execution times may contain error. Some real-time operating systems may also simplify the server behaviour to make implementation easier.

Therefore, a scheduling method that looks good in theory must still be checked carefully in practical implementation.

---

## 5. Assumptions and Embedded-System Consequences

The approaches discussed here rely on several assumptions. The classical Sporadic Server is mainly discussed for a single processor. It also assumes preemptive fixed-priority scheduling, known or bounded worst-case execution times, correct priority assignment, suitable server capacity and period, correct budget accounting, and correct replenishment handling.

These assumptions matter. If the system is multiprocessor, if execution times are unknown, or if timers are imprecise, then the analysis can become more difficult or may no longer apply directly.

For embedded systems, this is important because CPU budget and memory are limited. A more advanced server can improve response time, but it also requires more implementation effort. The system must support timers, queues, context switching, and budget tracking. Therefore, choosing a server method is not only a theoretical decision. It is also an architecture and design decision.

This fits the embedded-system view from the HW/SW co-design lecture, where scheduling is part of the overall design process and not isolated from implementation.

---

## 6. Why Related Work May Be Indirect

Searching only for the exact term “Sporadic Server” may give fewer results than expected. This does not mean the problem is unimportant. The reason is that Sporadic Server is one specific classical mechanism inside a broader area.

Related work may appear under broader terms such as:

* aperiodic task scheduling,
* sporadic task scheduling,
* server-based scheduling,
* resource reservation,
* fixed-priority scheduling,
* bandwidth reservation.

For this reason, I should not include unrelated papers just to increase the number of references. The better approach is to include sources that are genuinely connected to the same problem: serving irregular jobs while protecting real-time guarantees.

---

---

## 7. References and Source-use Notes

### [1] Sprunt, Sha, and Lehoczky — “Aperiodic Task Scheduling for Hard-Real-Time Systems”

Main source for the original Sporadic Server idea and motivation. Used for historical attribution and for understanding the general design goal of the mechanism.

### [2] Resource Reservation Server Lecture Material

Used for comparing Polling Server, Deferrable Server, and Sporadic Server.

### [3] HSHL Real-Time Systems Seminar Slides

Used for seminar requirements, scientific writing rules, GitHub usage, referencing expectations, and plagiarism policy.

### [4] HW/SW Co-Design Lecture

Used only as context for embedded-system design, especially the idea that scheduling is part of architecture and implementation decisions.

### [5] Buttazzo — “Hard Real-Time Computing Systems”

Recommended textbook reference. Currently not directly accessible, so no specific claims, page numbers, or quantitative results are taken from it. It should be consulted and cited properly if access becomes available.

---

## Source-use Note

This document is written in my own words as a Milestone 2 working document. The original Sporadic Server attribution is based on [1]. The comparison of server approaches is based on course and lecture material [2] and [3]. The embedded-system framing is supported by [4]. Exact utilisation bounds, formulas, and response-time analysis are intentionally kept qualitative here and should be verified from accessible primary sources before the final seminar paper.
