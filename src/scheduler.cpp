#include <iostream>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include "../include/scheduler.h"
#include "../include/utils.h"

Scheduler::Scheduler(std::vector<Process> processes,
                     Algorithm algo,
                     int quantum,
                     int contextSwitchTime)
    : processes(processes),
      algo(algo),
      quantum(quantum),
      current_time(0),
      cpu_busy_time(0),
      context_switch_time(contextSwitchTime) {}

void Scheduler::run() {
    current_time = 0;
    cpu_busy_time = 0;
    gantt.clear();

    // IMPORTANT: sort by arrival time
    std::sort(processes.begin(), processes.end(),
              [](const Process& a, const Process& b) {
                  return a.arrival_time < b.arrival_time;
              });

    for (auto &p : processes) {
        p.start_time = -1;
        p.remaining_time = p.burst_time;
        p.completion_time = 0;
        p.waiting_time = 0;
        p.turnaround_time = 0;
        p.is_waiting_for_io = false;
        p.io_remaining = 0;
        p.current_queue = 1;
        p.wait_counter = 0;
        p.state = NEW;
    }

    switch (algo) {
        case FCFS:        run_fcfs(); break;
        case SJF:         run_sjf(); break;
        case PRIORITY:    run_priority(); break;
        case ROUND_ROBIN: run_rr(quantum); break;
        case MLFQ:        run_mlfq(); break;
    }
}

/* ================= FCFS ================= */

void Scheduler::run_fcfs() {
    for (auto &p : processes) {
        while (current_time < p.arrival_time) {
            gantt.push_back("IDLE");
            current_time++;
        }

        if (context_switch_time > 0 && !gantt.empty()) {
            for (int i = 0; i < context_switch_time; i++) {
                gantt.push_back("CS");
                current_time++;
            }
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

/* ================= SJF (Preemptive) ================= */

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

/* ================= PRIORITY ================= */

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

/* ================= ROUND ROBIN ================= */

void Scheduler::run_rr(int quantum) {
    int n = processes.size();
    int completed = 0;
    std::queue<int> q;
    std::vector<bool> in_queue(n, false);

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {
                q.push(i);
                in_queue[i] = true;
            }
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

/* ================= MLFQ ================= */

void Scheduler::run_mlfq() {
    int n = processes.size();
    int completed = 0;

    std::queue<int> q1, q2, q3;
    const int q1_quantum = 2;
    const int q2_quantum = 4;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                processes[i].start_time == -1) {
                q1.push(i);
            }
        }

        int idx = -1, level = -1;
        if (!q1.empty()) idx = q1.front(), q1.pop(), level = 1;
        else if (!q2.empty()) idx = q2.front(), q2.pop(), level = 2;
        else if (!q3.empty()) idx = q3.front(), q3.pop(), level = 3;
        else {
            gantt.push_back("IDLE");
            current_time++;
            continue;
        }

        Process &p = processes[idx];
        if (p.start_time == -1) p.start_time = current_time;

        int exec = p.remaining_time;
        if (level == 1) exec = std::min(q1_quantum, p.remaining_time);
        else if (level == 2) exec = std::min(q2_quantum, p.remaining_time);

        for (int i = 0; i < exec; i++) {
            gantt.push_back(p.pid);
            current_time++;
            cpu_busy_time++;
            p.remaining_time--;
        }

        if (p.remaining_time == 0) {
            p.completion_time = current_time;
            completed++;
        } else {
            if (level == 1) q2.push(idx);
            else q3.push(idx);
        }
    }

    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

/* ================= RESULTS ================= */

void Scheduler::print_results() {
    print_table(processes);
    print_gantt(gantt);
    export_gantt_csv(gantt);
    print_system_metrics(current_time, cpu_busy_time, processes.size());
}
