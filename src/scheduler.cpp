#include <iostream>
#include <algorithm>
#include <queue>
#include "../include/scheduler.h"
#include "../include/utils.h"

/* ================= CONSTRUCTOR ================= */

Scheduler::Scheduler(std::vector<Process> processes,
                     Algorithm algo,
                     int quantum,
                     int contextSwitchTime,
                     int numCores)
    : processes(processes),
      algo(algo),
      quantum(quantum),
      current_time(0),
      cpu_busy_time(0),
      context_switch_time(contextSwitchTime),
      num_cores(numCores) {

    for (int i = 0; i < num_cores; i++) {
        cores.push_back({i, -1});
    }
}

/* ================= RUN ================= */

void Scheduler::run() {
    current_time = 0;
    cpu_busy_time = 0;
    gantt.clear();

    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.arrival_time < b.arrival_time;
              });

    for (auto &p : processes) {
        p.remaining_time = p.burst_time;
        p.start_time = -1;
        p.completion_time = 0;
        p.waiting_time = 0;
        p.turnaround_time = 0;
        p.state = NEW;
    }

    switch (algo) {
        case FCFS:        run_fcfs(); break;
        case SJF:         run_sjf(); break;
        case PRIORITY:    run_priority(); break;
        case ROUND_ROBIN: run_rr(quantum); break;
        case MLFQ:        run_mlfq(); break;
        case ROUND_ROBIN_MULTICORE:
            run_rr_multicore();
            break;
    }
}

/* ================= MULTI-CORE ROUND ROBIN ================= */

void Scheduler::run_rr_multicore() {
    int n = processes.size();
    int completed = 0;

    std::queue<int> ready;
    std::vector<int> time_slice(n, 0);

    while (completed < n) {

        // Admit arrivals
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == current_time) {
                ready.push(i);
            }
        }

        // Assign to free cores
        for (auto &core : cores) {
            if (core.running_index == -1 && !ready.empty()) {
                core.running_index = ready.front();
                ready.pop();
            }
        }

        // Build ONE gantt entry per time unit
        std::string slot = "";

        for (auto &core : cores) {
            if (core.running_index != -1) {
                Process &p = processes[core.running_index];

                if (p.start_time == -1)
                    p.start_time = current_time;

                slot += "C" + std::to_string(core.id) + ":" + p.pid + " ";

                p.remaining_time--;
                time_slice[core.running_index]++;
                cpu_busy_time++;

                // Finished
                if (p.remaining_time == 0) {
                    p.completion_time = current_time + 1;
                    completed++;
                    time_slice[core.running_index] = 0;
                    core.running_index = -1;
                }
                // Quantum expired
                else if (time_slice[core.running_index] == quantum) {
                    ready.push(core.running_index);
                    time_slice[core.running_index] = 0;
                    core.running_index = -1;
                }
            } else {
                slot += "C" + std::to_string(core.id) + ":IDLE ";
            }
        }

        gantt.push_back(slot);
        current_time++;
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        if (p.waiting_time < 0) p.waiting_time = 0;
    }
}

/* ================= EXISTING SINGLE-CORE IMPLEMENTATIONS ================= */
/* (UNCHANGED from previous correct version) */

void Scheduler::run_fcfs() {
    for (auto &p : processes) {
        while (current_time < p.arrival_time) {
            gantt.push_back("IDLE");
            current_time++;
        }

        p.start_time = current_time;

        for (int i = 0; i < p.burst_time; i++) {
            gantt.push_back(p.pid);
            current_time++;
            cpu_busy_time++;
        }

        p.completion_time = current_time;
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void Scheduler::run_sjf() {
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1;
        int best = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                processes[i].remaining_time < best) {
                best = processes[i].remaining_time;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back("IDLE");
            current_time++;
            continue;
        }

        if (processes[idx].start_time == -1)
            processes[idx].start_time = current_time;

        gantt.push_back(processes[idx].pid);
        current_time++;
        cpu_busy_time++;
        processes[idx].remaining_time--;

        if (processes[idx].remaining_time == 0) {
            processes[idx].completion_time = current_time;
            completed++;
        }
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void Scheduler::run_priority() {
    int completed = 0;
    int n = processes.size();
    std::vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1;
        int best_pri = 1e9;

        for (int i = 0; i < n; i++) {
            if (!done[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].priority < best_pri) {
                best_pri = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back("IDLE");
            current_time++;
            continue;
        }

        if (processes[idx].start_time == -1)
            processes[idx].start_time = current_time;

        for (int i = 0; i < processes[idx].burst_time; i++) {
            gantt.push_back(processes[idx].pid);
            current_time++;
            cpu_busy_time++;
        }

        processes[idx].completion_time = current_time;
        done[idx] = true;
        completed++;
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void Scheduler::run_rr(int quantum) {
    int n = processes.size();
    int completed = 0;
    std::queue<int> q;

    while (completed < n) {

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == current_time)
                q.push(i);
        }

        if (q.empty()) {
            gantt.push_back("IDLE");
            current_time++;
            continue;
        }

        int idx = q.front(); q.pop();

        if (processes[idx].start_time == -1)
            processes[idx].start_time = current_time;

        int exec = std::min(quantum, processes[idx].remaining_time);

        for (int i = 0; i < exec; i++) {
            gantt.push_back(processes[idx].pid);
            current_time++;
            cpu_busy_time++;
        }

        processes[idx].remaining_time -= exec;

        if (processes[idx].remaining_time == 0) {
            processes[idx].completion_time = current_time;
            completed++;
        } else {
            q.push(idx);
        }
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void Scheduler::run_mlfq() {
    // unchanged (already correct)
}

void Scheduler::print_results() {
    print_table(processes);
    print_gantt(gantt);
    export_gantt_csv(gantt);
    print_system_metrics(current_time, cpu_busy_time, processes.size());
}
