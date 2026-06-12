#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

/*
  Fixed-priority Priority Exchange (PE) server simulator.

  Purpose for RTS seminar:
  - Demonstrates the MAIN FEATURE of Priority Exchange:
      1) server replenishment,
      2) aperiodic service when budget is available,
      3) exchange of unused high-priority budget with periodic execution,
      4) parking/degrading budget at lower priority levels,
      5) budget discard only when the processor would otherwise be idle.

  Model:
  - Discrete unit-time simulation.
  - One CPU, preemptive fixed-priority scheduling.
  - Level 0 = PE server priority (highest).
  - Level 1..n = periodic tasks ordered by Rate Monotonic priority.
  - Smaller level number means higher priority.

  Example matches the paper/slides:
  - Server: Cs = 1, Ts = 5
  - tau1 = (C1 = 2, T1 = 10)
  - tau2 = (C2 = 6, T2 = 20)
  - J1 arrives at t = 5, execution = 1
  - J2 arrives at t = 12, execution = 1
*/

struct PeriodicTask {
    std::string name;
    int C;              // WCET per job
    int T;              // period = relative deadline
    int remaining = 0;  // current job remaining execution
    int deadline = 0;   // current absolute deadline
    int level = 0;      // fixed-priority level: 1 = highest periodic priority
    int jobs_released = 0;
    int deadline_misses = 0;
};

struct AperiodicJob {
    std::string name;
    int arrival;
    int execution;
    int remaining;
    int finish = -1;
};

std::string cap_string(const std::vector<int>& cap) {
    std::ostringstream os;
    os << "[S:" << cap[0];
    for (size_t i = 1; i < cap.size(); ++i) {
        os << ", tau" << i << ":" << cap[i];
    }
    os << "]";
    return os.str();
}

std::string rem_string(const std::vector<PeriodicTask>& tasks) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (i) os << ", ";
        os << tasks[i].name << ":" << tasks[i].remaining;
    }
    os << "]";
    return os.str();
}

int first_ready_periodic_level(const std::vector<PeriodicTask>& tasks) {
    int best = 1000000;
    for (const auto& task : tasks) {
        if (task.remaining > 0) best = std::min(best, task.level);
    }
    return best == 1000000 ? -1 : best;
}

int first_ready_periodic_strictly_below(const std::vector<PeriodicTask>& tasks, int budget_level) {
    int best = 1000000;
    for (const auto& task : tasks) {
        if (task.remaining > 0 && task.level > budget_level) {
            best = std::min(best, task.level);
        }
    }
    return best == 1000000 ? -1 : best;
}

int first_budget_level(const std::vector<int>& cap) {
    for (int level = 0; level < static_cast<int>(cap.size()); ++level) {
        if (cap[level] > 0) return level;
    }
    return -1;
}

int main() {
    const int Cs = 1;       // PE server capacity
    const int Ts = 5;       // PE server period
    const int T_END = 20;   // simulate [0, 20)

    // Periodic tasks are already RM sorted: shorter period = higher priority.
    std::vector<PeriodicTask> tasks = {
        {"tau1", 2, 10, 0, 0, 1},
        {"tau2", 6, 20, 0, 0, 2}
    };

    std::vector<AperiodicJob> all_jobs = {
        {"J1", 5, 1, 1},
        {"J2", 12, 1, 1}
    };

    std::queue<int> ap_queue;  // stores indices of all_jobs
    std::vector<int> cap(tasks.size() + 1, 0);  // cap[0] server, cap[i] parked at tau_i level

    std::cout << "Priority Exchange fixed-priority unit-time simulation\n";
    std::cout << "Server: Cs=" << Cs << ", Ts=" << Ts << "\n";
    std::cout << "Periodic tasks: tau1=(C=2,T=10), tau2=(C=6,T=20)\n";
    std::cout << "Aperiodic jobs: J1=(arrival=5,C=1), J2=(arrival=12,C=1)\n\n";

    std::cout << std::left
              << std::setw(5)  << "t"
              << std::setw(26) << "events"
              << std::setw(48) << "CPU action in [t,t+1)"
              << std::setw(24) << "budget after action"
              << std::setw(20) << "periodic remaining"
              << "\n";
    std::cout << std::string(125, '-') << "\n";

    for (int t = 0; t < T_END; ++t) {
        std::ostringstream events;

        // Deadline check before new release at the same time instant.
        for (auto& task : tasks) {
            if (t > 0 && t == task.deadline && task.remaining > 0) {
                task.deadline_misses++;
                events << task.name << " DEADLINE MISS; ";
            }
        }

        // Release periodic jobs.
        for (auto& task : tasks) {
            if (t % task.T == 0) {
                if (task.remaining > 0) {
                    task.deadline_misses++;
                    events << task.name << " overlap/miss; ";
                }
                task.remaining += task.C;
                task.deadline = t + task.T;
                task.jobs_released++;
                events << "rel " << task.name << "; ";
            }
        }

        // Release aperiodic jobs.
        for (size_t j = 0; j < all_jobs.size(); ++j) {
            if (all_jobs[j].arrival == t) {
                ap_queue.push(static_cast<int>(j));
                events << "arr " << all_jobs[j].name << "; ";
            }
        }

        // Server replenishment. New top-priority capacity appears every Ts.
        // Existing parked lower-priority capacity is not erased.
        if (t % Ts == 0) {
            cap[0] += Cs;
            events << "repl Cs; ";
        }

        std::ostringstream action;
        bool ap_pending = !ap_queue.empty();

        int periodic_level = first_ready_periodic_level(tasks);
        int chosen_level = 1000000;
        std::string chosen_type;

        // Candidate 1: a periodic task at its own level.
        if (periodic_level != -1) {
            chosen_level = periodic_level;
            chosen_type = "periodic";
        }

        // Candidate 2: if aperiodic job is pending, any budget can serve it at its level.
        if (ap_pending) {
            int b = first_budget_level(cap);
            if (b != -1 && b <= chosen_level) { // tie goes to aperiodic service
                chosen_level = b;
                chosen_type = "serve";
            }
        }

        // Candidate 3: if no aperiodic job is pending, a budget can exchange with
        // the highest-priority active periodic task below that budget level.
        if (!ap_pending) {
            for (int b = 0; b < static_cast<int>(cap.size()); ++b) {
                if (cap[b] > 0 && first_ready_periodic_strictly_below(tasks, b) != -1) {
                    if (b < chosen_level) {
                        chosen_level = b;
                        chosen_type = "exchange";
                    }
                    break; // first budget is the highest-priority budget
                }
            }
        }

        if (chosen_type == "serve") {
            int job_index = ap_queue.front();
            cap[chosen_level]--;
            all_jobs[job_index].remaining--;
            action << "serve " << all_jobs[job_index].name
                   << " using budget at level " << chosen_level;
            if (all_jobs[job_index].remaining == 0) {
                all_jobs[job_index].finish = t + 1;
                ap_queue.pop();
                action << " (finished)";
            }
        } else if (chosen_type == "exchange") {
            int task_level = first_ready_periodic_strictly_below(tasks, chosen_level);
            auto it = std::find_if(tasks.begin(), tasks.end(), [&](const PeriodicTask& task) {
                return task.level == task_level;
            });
            cap[chosen_level]--;
            cap[task_level]++;
            it->remaining--;
            action << "EXCHANGE: budget level " << chosen_level
                   << " -> run " << it->name
                   << "; park 1 budget at level " << task_level;
        } else if (chosen_type == "periodic") {
            auto it = std::find_if(tasks.begin(), tasks.end(), [&](const PeriodicTask& task) {
                return task.level == chosen_level;
            });
            it->remaining--;
            action << "run " << it->name << " normally at level " << chosen_level;
        } else {
            int b = first_budget_level(cap);
            if (b != -1) {
                cap[b]--;
                action << "IDLE: discard 1 parked budget at level " << b;
            } else {
                action << "IDLE: no work and no budget";
            }
        }

        std::cout << std::left
                  << std::setw(5)  << t
                  << std::setw(26) << events.str()
                  << std::setw(48) << action.str()
                  << std::setw(24) << cap_string(cap)
                  << std::setw(20) << rem_string(tasks)
                  << "\n";
    }

    std::cout << "\nAperiodic response times:\n";
    for (const auto& job : all_jobs) {
        if (job.finish >= 0) {
            std::cout << "  " << job.name << ": arrival=" << job.arrival
                      << ", finish=" << job.finish
                      << ", response=" << (job.finish - job.arrival) << "\n";
        } else {
            std::cout << "  " << job.name << ": not finished\n";
        }
    }

    std::cout << "\nDeadline check:\n";
    for (const auto& task : tasks) {
        std::cout << "  " << task.name << ": released=" << task.jobs_released
                  << ", deadline_misses=" << task.deadline_misses << "\n";
    }

    std::cout << "\nImportant points to explain in the presentation:\n";
    std::cout << "  t=0  : fresh server budget is exchanged with tau1.\n";
    std::cout << "  t=5  : J1 arrives and is served immediately with fresh top-priority budget.\n";
    std::cout << "  t=9  : old parked budget is discarded only because the CPU would be idle.\n";
    std::cout << "  t=12 : J2 is served using reclaimed budget parked at tau1's priority level.\n";

    return 0;
}
