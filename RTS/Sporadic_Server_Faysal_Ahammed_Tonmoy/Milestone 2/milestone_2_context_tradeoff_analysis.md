Milestone 2: Scientific Contextualization and Tradeoff Analysis

Topic: Sporadic Server

Name: Faysal Ahammed Tonmoy

Matriculation No.: 1230473

Team: A1

Course: Real-Time Systems

1. Purpose of this Milestone
Milestone 1 was about understanding the Sporadic Server itself — what it is and how its budget and replenishment work. Milestone 2 does not repeat that. Instead it places the Sporadic Server in a wider context: which other scheduling approaches deal with the same problem, what assumptions they rely on, and what tradeoffs follow from choosing one over another. So Milestone 1 answered what a Sporadic Server is, and Milestone 2 looks at why it was proposed, what the alternatives are, and what the choice means for a real embedded system.
2. Position of Sporadic Server
The Sporadic Server is a way of handling aperiodic and sporadic tasks inside a fixed-priority, preemptive system — usually one where the periodic tasks use Rate Monotonic (RM) priorities. It is a server-based (bandwidth-preserving) method: a fixed amount of processor time is reserved at a chosen priority and used to run jobs that do not arrive on a regular period.
It is worth saying what it is not. It is not a full scheduling policy and not a replacement for RM or EDF. It runs on top of a base policy and coexists with the periodic tasks. Its purpose is narrow: to give irregular jobs bounded, prioritised processor time so they get a reasonable response, without breaking the timing guarantees of the periodic tasks. The mechanism was originally introduced by Sprunt, Sha, and Lehoczky [1].
3. Related and Neighboring Approaches
The approaches below are included because they deal with the same problem — servicing aperiodic work alongside periodic real-time tasks — or because they are the closest technical neighbours of the Sporadic Server. Loosely related work has been left out on purpose, following the seminar guidance [3].
3.1 Background Scheduling
This is the simplest option. Aperiodic jobs run at the lowest priority, so only in the processor's idle time. Because they never preempt periodic tasks, they have no effect on periodic schedulability. The downside is response time: when the periodic load is high there is little idle time, so an aperiodic job can wait a long time, and the wait is hard to bound. The Sporadic Server can be seen as an improvement on this, giving aperiodic work a dedicated higher-priority budget instead of leaving it to idle time.
3.2 Polling Server
A Polling Server is a periodic task that serves pending aperiodic requests at each activation, up to its capacity. If nothing is waiting at the polling instant, the capacity for that period is discarded. It is simple, and since it is literally periodic it fits normal periodic schedulability analysis. The weakness is timing: a request arriving just after a polling instant waits until the next activation, which in the worst case is close to a full period, and capacity is wasted when there is nothing to do. From my current understanding, the Sporadic Server tries to keep better responsiveness than the Polling Server while still keeping interference bounded through its replenishment rule.
3.3 Deferrable Server
A Deferrable Server is also periodic but keeps its capacity through the period instead of discarding it. As long as budget remains, a request arriving at any time can be served immediately, and capacity is refilled at the start of each period. This gives better response than the Polling Server. The cost is more interference: because capacity can be spent late in one period and again early in the next, the server can put two bursts of demand close together on lower-priority tasks, which lowers the utilisation that the periodic set can be guaranteed. So a Deferrable Server does not behave like a normal periodic task in the analysis. As far as I understand, the Sporadic Server was designed to keep this kind of capacity preservation while still allowing cleaner reasoning about interference, through its replenishment rule [1].
3.4 Priority Exchange and Slack Stealing
These are two further approaches in the same area. Priority Exchange is another fixed-priority bandwidth-preserving server: instead of discarding unused capacity it "exchanges" it down to lower priority levels, so the capacity is preserved but at a reduced priority, at the cost of more bookkeeping. Slack stealing is different — it is not a fixed-capacity server but computes the slack of the periodic tasks (how much they can be delayed without missing deadlines) and uses it to run aperiodic jobs, often at high priority. It can give very good response times when the slack is computed correctly, but that computation is expensive. Both are included as related context rather than as the main comparison.
3.5 EDF-based Servers
The servers above assume fixed priorities. In the dynamic-priority (EDF) world there is a parallel family of bandwidth-reservation servers, for example the Total Bandwidth Server and the Constant Bandwidth Server (CBS), where CBS also gives temporal isolation between tasks. They are related as the EDF counterparts of bandwidth reservation, but they belong to a different scheduling paradigm, so I treat them as indirect context. The Sporadic Server is a fixed-priority mechanism.
4. Main Tradeoffs
4.1 Response Time vs Schedulability
These two goals work against each other, and the approaches above sit at different points on that line. Background scheduling protects the periodic tasks fully but gives the worst response. The Polling Server keeps the analysis clean but pays a response-time penalty. The Deferrable Server improves response but lowers the schedulable utilisation because of the extra interference. From my current understanding, the main strength of the Sporadic Server is that it tries to keep good aperiodic response while still allowing cleaner schedulability reasoning. It is not always the best choice though — depending on the parameters and the actual arrival pattern, a Deferrable Server or slack stealing can give better response in some cases.
4.2 Simplicity vs Implementation Complexity
Among the capacity-preserving servers the Polling Server is the simplest, since there is little to track beyond a periodic refill. The Deferrable Server is a bit more involved but still uses a single periodic refill. The Sporadic Server is the most demanding: it has to track how much budget was used and schedule each replenishment by its rule, which can mean several pending replenishments at once. More control over timing means more state and more logic to implement and test.
4.3 Runtime and Memory Overhead
The extra control of a Sporadic Server costs something at runtime. The main sources are:

budget checking before and during dispatch,
updating the remaining budget as the server runs,
managing the replenishment queue (several future replenishments, each needing storage and a timer),
managing the queue of waiting aperiodic jobs,
timer/interrupt events when replenishments are due,
extra context switches when the server preempts or yields.

On a small embedded target, where memory and CPU cycles are limited, these are not negligible and can eat into the responsiveness the server was chosen for.
4.4 Formal Guarantees vs Practical Implementation
The guarantees above rest on idealised assumptions: perfect preemption, negligible scheduler overhead, exact timing, correct worst-case execution times, and a correctly implemented replenishment rule. Real systems differ — timer resolution is finite, interrupt latency is non-zero, the scheduler itself takes time, measured execution times have error, and some RTOS implementations simplify the replenishment rule. So a method that looks good on paper may behave differently once deployed, and for embedded work the practical deployability and the correctness of the implementation matter as much as the theoretical bound. The same gap shows up between offline analysis and online arrivals: periodic tasks can be analysed before runtime, while aperiodic jobs arrive online, so the server is dimensioned offline but has to react online within its reserved budget.
5. Assumptions and Embedded-System Consequences
The servers here share a set of assumptions: a single processor (the classical Sporadic Server is uniprocessor), a preemptive fixed-priority base scheduler, known or bounded worst-case execution times for the periodic tasks, correct priority assignment (for example RM), a sensible server period and capacity, correct budget accounting and replenishment, and aperiodic jobs that can be queued and can tolerate some delay. These are not free — moving to a multiprocessor, allowing unknown execution times, or accepting imprecise timers changes the analysis and can break the guarantees.
For embedded systems this matters in practice. CPU budget and memory are limited, so the overhead in Section 4.3 is a real factor. Timer precision and scheduler overhead decide whether a server actually behaves as its analysis predicts. The more capable mechanisms (Sporadic Server, slack stealing) buy responsiveness with implementation effort and runtime cost, which has to be justified by the application. This is why the choice of scheduling mechanism is really a design and architecture decision, not only a theoretical one, which fits the HW/SW co-design view [4]. The right server depends on the mix of periodic and aperiodic work, how critical the aperiodic jobs are, and the resource budget. Many embedded and edge systems run periodic control or sensing loops next to irregular, event-driven processing, which is exactly the situation these servers are meant for.
6. Why Related Work May Be Indirect
Searching for "Sporadic Server" alone gives few results, which can look like the topic is barely studied. The reason is that it is one specific classical mechanism, and most of the related literature sits under broader headings such as aperiodic and sporadic task scheduling, server-based scheduling, resource reservation, fixed-priority scheduling, and bandwidth reservation. Searching under those terms gives a fuller picture. Following the seminar guidance, I have not added unrelated papers just to make the list longer [3].
7. Open Questions

Should the final paper focus the detailed comparison on Polling, Deferrable, and Sporadic Server, and keep Priority Exchange, slack stealing, and the EDF servers only as brief context?
How deep should the replenishment-rule explanation be — conceptual, or a worked step-by-step example?
Should the implementation example be modelled in UPPAAL or written in C/C++ on an RTOS such as FreeRTOS?
If the recommended textbook stays inaccessible, are the lecture materials acceptable as the main supporting sources?
How much quantitative detail (utilisation bounds, response-time formulas) is expected, given that exact figures should come from a primary source I cannot currently access?

8. References and Source-use Notes
[1] Sprunt, B., Sha, L., and Lehoczky, J. — "Aperiodic Task Scheduling for Hard-Real-Time Systems", The Journal of Real-Time Systems, vol. 1, no. 1, 1989.
Main source for the original Sporadic Server idea and motivation. Used for the historical attribution and the general design goal. No specific quantitative results have been reproduced; bibliographic details (volume, pages) should be confirmed from the accessible copy.
[2] Resource Reservation Server — Lecture Material.
Course source for comparing Polling, Deferrable, and Sporadic Server. Used as the basis for Sections 3 and 4.
[3] HSHL Real-Time Systems Seminar Slides.
Process/context source: seminar requirements, scientific writing rules, GitHub usage, referencing, and the plagiarism policy, including the guidance not to pad the reference list.
[4] HW/SW Co-Design Lecture.
Context source for the embedded framing in Section 5 — scheduling as a design/architecture decision.
[5] Buttazzo, G. — "Hard Real-Time Computing Systems".
Recommended textbook, not accessible at this stage. No specific claims, page numbers, or results are taken from it anywhere in this document. To be consulted and cited properly if access is obtained.
Source-use note: the text is written in my own words from general real-time scheduling knowledge. Attribution for the mechanism is to [1]; the comparison follows the course materials [2] and [3]; the embedded framing follows [4]. Bounds and exact formulas are kept qualitative on purpose and should be checked against an accessible primary source before final submission.
