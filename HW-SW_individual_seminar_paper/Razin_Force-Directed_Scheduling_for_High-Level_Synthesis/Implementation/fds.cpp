#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

enum ResourceType { MUL = 0, ADD = 1 };

string type_name(int t) { return t == MUL ? "MUL" : "ADD"; }
string op_symbol(int t) { return t == MUL ? "*" : "+"; }

struct Operation {
    string name;
    int type;
    vector<int> pred;
    vector<int> succ;
};

struct Problem {
    int latency = 0;
    vector<Operation> ops;

    int add_op(const string& name, int type) {
        ops.push_back(Operation{name, type, {}, {}});
        return (int)ops.size() - 1;
    }

    void add_edge(int from, int to) {
        ops[from].succ.push_back(to);
        ops[to].pred.push_back(from);
    }

    int n() const {
        return (int)ops.size();
    }
};

struct Frames {
    vector<int> asap;
    vector<int> alap;
    bool feasible = true;
    string reason;
};

vector<int> topological_order(const Problem& p) {
    vector<int> indeg(p.n(), 0);

    for (int i = 0; i < p.n(); ++i) {
        for (int s : p.ops[i].succ) {
            indeg[s]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < p.n(); ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    vector<int> order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : p.ops[u].succ) {
            indeg[v]--;
            if (indeg[v] == 0) q.push(v);
        }
    }

    if ((int)order.size() != p.n()) {
        throw runtime_error("DFG has a cycle. FDS expects a DAG.");
    }

    return order;
}

Frames compute_frames(const Problem& p, const vector<int>& fixed_step) {
    int n = p.n();

    Frames f;
    f.asap.assign(n, 1);
    f.alap.assign(n, p.latency);

    vector<int> order = topological_order(p);

    // Forward pass: ASAP / earliest possible step
    for (int u : order) {
        int earliest = 1;

        for (int pr : p.ops[u].pred) {
            earliest = max(earliest, f.asap[pr] + 1);
        }

        if (fixed_step[u] != -1) {
            if (fixed_step[u] < earliest) {
                f.feasible = false;
                f.reason = p.ops[u].name + " fixed before predecessor is ready";
                return f;
            }
            f.asap[u] = fixed_step[u];
        } else {
            f.asap[u] = earliest;
        }
    }

    // Backward pass: ALAP / latest possible step
    reverse(order.begin(), order.end());

    for (int u : order) {
        int latest = p.latency;

        for (int su : p.ops[u].succ) {
            latest = min(latest, f.alap[su] - 1);
        }

        if (fixed_step[u] != -1) {
            if (fixed_step[u] > latest) {
                f.feasible = false;
                f.reason = p.ops[u].name + " fixed after successor needs it";
                return f;
            }
            f.alap[u] = fixed_step[u];
        } else {
            f.alap[u] = latest;
        }
    }

    for (int u = 0; u < n; ++u) {
        if (f.asap[u] > f.alap[u]) {
            f.feasible = false;
            f.reason = "No legal time frame for " + p.ops[u].name;
            return f;
        }
    }

    return f;
}

double probability(const Frames& fr, int op, int step) {
    if (!fr.feasible) return 0.0;

    if (step < fr.asap[op] || step > fr.alap[op]) {
        return 0.0;
    }

    return 1.0 / double(fr.alap[op] - fr.asap[op] + 1);
}

vector<vector<double>> distribution_graph(const Problem& p, const Frames& fr) {
    vector<vector<double>> dg(2, vector<double>(p.latency + 1, 0.0));

    for (int u = 0; u < p.n(); ++u) {
        int type = p.ops[u].type;

        for (int s = 1; s <= p.latency; ++s) {
            dg[type][s] += probability(fr, u, s);
        }
    }

    return dg;
}

// MAIN FEATURE:
// Force computation for candidate pair (operation, step).
// It pins one operation, recomputes frames, and sums the DG-weighted
// probability change. Self-force and predecessor/successor force are
// included automatically because all affected frames are recomputed.
double force(const Problem& p,
             const vector<int>& fixed_step,
             int op,
             int step) {
    Frames before = compute_frames(p, fixed_step);
    if (!before.feasible) {
        return numeric_limits<double>::infinity();
    }

    if (step < before.asap[op] || step > before.alap[op]) {
        return numeric_limits<double>::infinity();
    }

    vector<vector<double>> dg = distribution_graph(p, before);

    vector<int> trial = fixed_step;
    trial[op] = step;

    Frames after = compute_frames(p, trial);
    if (!after.feasible) {
        return numeric_limits<double>::infinity();
    }

    double total_force = 0.0;

    for (int u = 0; u < p.n(); ++u) {
        if (fixed_step[u] != -1 && u != op) {
            continue;
        }

        int type = p.ops[u].type;

        for (int s = 1; s <= p.latency; ++s) {
            double oldP = probability(before, u, s);
            double newP = probability(after, u, s);
            total_force += dg[type][s] * (newP - oldP);
        }
    }

    return total_force;
}

void print_frames(const Problem& p, const Frames& fr) {
    cout << "\nTime frames (ASAP/ALAP):\n";
    cout << "Operation  Type  ASAP  ALAP  Mobility\n";

    for (int u = 0; u < p.n(); ++u) {
        cout << left << setw(10) << p.ops[u].name
             << setw(6) << type_name(p.ops[u].type)
             << setw(6) << fr.asap[u]
             << setw(6) << fr.alap[u]
             << (fr.alap[u] - fr.asap[u]) << "\n";
    }
}

void print_dg(const Problem& p,
              const vector<vector<double>>& dg,
              const string& title) {
    cout << "\n" << title << "\n";
    cout << fixed << setprecision(3);

    for (int type = 0; type <= 1; ++type) {
        cout << type_name(type) << ": ";

        for (int s = 1; s <= p.latency; ++s) {
            cout << "s" << s << "=" << dg[type][s];

            if (s != p.latency) {
                cout << ", ";
            }
        }

        cout << "\n";
    }

    cout.unsetf(ios::fixed);
}

vector<int> asap_schedule(const Problem& p) {
    vector<int> fixed_step(p.n(), -1);
    Frames fr = compute_frames(p, fixed_step);
    return fr.asap;
}

map<int, int> resource_peaks(const Problem& p, const vector<int>& schedule) {
    map<int, int> peak;
    peak[MUL] = 0;
    peak[ADD] = 0;

    for (int s = 1; s <= p.latency; ++s) {
        map<int, int> count;

        for (int u = 0; u < p.n(); ++u) {
            if (schedule[u] == s) {
                count[p.ops[u].type]++;
            }
        }

        for (auto item : count) {
            peak[item.first] = max(peak[item.first], item.second);
        }
    }

    return peak;
}

void print_schedule(const Problem& p,
                    const vector<int>& schedule,
                    const string& title) {
    cout << "\n" << title << "\n";

    for (int s = 1; s <= p.latency; ++s) {
        cout << "s" << s << ": ";
        bool empty = true;

        for (int u = 0; u < p.n(); ++u) {
            if (schedule[u] == s) {
                if (!empty) {
                    cout << ", ";
                }

                cout << p.ops[u].name
                     << "(" << op_symbol(p.ops[u].type) << ")";

                empty = false;
            }
        }

        if (empty) {
            cout << "--";
        }

        cout << "\n";
    }

    map<int, int> peak = resource_peaks(p, schedule);
    cout << "Resource peaks: MUL=" << peak[MUL]
         << ", ADD=" << peak[ADD] << "\n";
}

vector<int> run_fds(const Problem& p) {
    vector<int> fixed_step(p.n(), -1);

    cout << "\nFDS decisions:\n";

    for (int iter = 1; iter <= p.n(); ++iter) {
        Frames fr = compute_frames(p, fixed_step);
        vector<vector<double>> dg = distribution_graph(p, fr);

        double best_force = numeric_limits<double>::infinity();
        int best_op = -1;
        int best_step = -1;

        for (int u = 0; u < p.n(); ++u) {
            if (fixed_step[u] != -1) {
                continue;
            }

            for (int s = fr.asap[u]; s <= fr.alap[u]; ++s) {
                double f = force(p, fixed_step, u, s);

                bool better = false;

                if (f < best_force - 1e-9) {
                    better = true;
                } else if (abs(f - best_force) <= 1e-9) {
                    double candidate_load = dg[p.ops[u].type][s];

                    double best_load =
                        best_op == -1
                            ? numeric_limits<double>::infinity()
                            : dg[p.ops[best_op].type][best_step];

                    if (candidate_load < best_load - 1e-9) {
                        better = true;
                    } else if (abs(candidate_load - best_load) <= 1e-9 &&
                               s > best_step) {
                        better = true;
                    }
                }

                if (better) {
                    best_force = f;
                    best_op = u;
                    best_step = s;
                }
            }
        }

        if (best_op == -1) {
            throw runtime_error("No feasible FDS decision found.");
        }

        fixed_step[best_op] = best_step;

        cout << "  Iter " << iter
             << ": fix " << p.ops[best_op].name
             << " -> s" << best_step
             << "   force=" << fixed << setprecision(4)
             << best_force << "\n";

        cout.unsetf(ios::fixed);
    }

    return fixed_step;
}

int main() {
    Problem p;
    p.latency = 4;

    // 5-operation paper example:
    // 3 multiplications + 2 additions
    int v1 = p.add_op("v1", MUL);
    int v2 = p.add_op("v2", MUL);
    int v3 = p.add_op("v3", MUL);
    int v4 = p.add_op("v4", ADD);
    int v5 = p.add_op("v5", ADD);

    // Dependencies:
    // v1 -> v4
    // v2 -> v4
    // v4 -> v5
    // v3 -> v5
    p.add_edge(v1, v4);
    p.add_edge(v2, v4);
    p.add_edge(v4, v5);
    p.add_edge(v3, v5);

    cout << "Force-Directed Scheduling demo\n";
    cout << "Latency bound = " << p.latency << " control steps\n";
    cout << "DFG = 3 multiplications + 2 additions\n";

    vector<int> none(p.n(), -1);

    Frames initial = compute_frames(p, none);

    if (!initial.feasible) {
        cerr << "Initial problem infeasible: "
             << initial.reason << "\n";
        return 1;
    }

    print_frames(p, initial);

    vector<vector<double>> initial_dg =
        distribution_graph(p, initial);

    print_dg(p, initial_dg, "Initial distribution graph (DG):");

    vector<int> asap = asap_schedule(p);
    print_schedule(p, asap, "ASAP schedule:");

    vector<int> fds = run_fds(p);
    print_schedule(p, fds, "Final FDS schedule:");

    Frames final_frames = compute_frames(p, fds);

    vector<vector<double>> final_dg =
        distribution_graph(p, final_frames);

    print_dg(p, final_dg, "Final fixed DG:");

    map<int, int> asap_peak = resource_peaks(p, asap);
    map<int, int> fds_peak = resource_peaks(p, fds);

    cout << "\nComparison:\n";
    cout << "ASAP multiplier peak = " << asap_peak[MUL] << "\n";
    cout << "FDS  multiplier peak = " << fds_peak[MUL] << "\n";
    cout << "ASAP adder peak      = " << asap_peak[ADD] << "\n";
    cout << "FDS  adder peak      = " << fds_peak[ADD] << "\n";

    return 0;
}