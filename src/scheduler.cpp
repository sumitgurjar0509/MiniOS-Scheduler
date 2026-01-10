#include <iostream>
#include <algorithm>
#include <queue>
#include "../include/scheduler.h"
#include "../include/utils.h"

Scheduler::Scheduler(std::vector<Process> processes, Algorithm algo, int quantum)
    : processes(processes), algo(algo), quantum(quantum) {}

void Scheduler::run() {
    switch (algo) {
        case FCFS: run_fcfs(); break;
        case SJF: run_sjf(); break;
        case PRIORITY: run_priority(); break;
        case ROUND_ROBIN: run_rr(); break;
    }
}

void Scheduler::run_fcfs() {
    int time = 0;
    for (auto &p : processes) {
        if (time < p.arrival_time) {
            // CPU idle
            while (time < p.arrival_time) {
                gantt.push_back("IDLE");
                time++;
            }
        }
        p.start_time = time;
        for (int i = 0; i < p.burst_time; i++)
            gantt.push_back(p.pid), time++;
        p.completion_time = time;
    }
}


void Scheduler::run_sjf() { // Preemptive (Shortest Remaining Time First)
    int time = 0;
    int completed = 0;
    int n = processes.size();

    // Initialize remaining_time if not set
    for (auto &p : processes)
        p.remaining_time = p.burst_time;

    while (completed < n) {
        int idx = -1;
        int min_rt = 1e9;

        // Find job with shortest remaining time at current time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_rt) {
                    min_rt = processes[i].remaining_time;
                    idx = i;
                }
            }
        }

        // CPU idle
        if (idx == -1) {
            gantt.push_back("IDLE");
            time++;
            continue;
        }

        // If first time running
        if (processes[idx].start_time == -1)
            processes[idx].start_time = time;

        // Execute one unit
        gantt.push_back(processes[idx].pid);
        processes[idx].remaining_time--;
        gantt.push_back(processes[idx].pid);
        time++;


        // Finished
        if (processes[idx].remaining_time == 0) {
            processes[idx].completion_time = time;
            completed++;
        }
    }
}


void Scheduler::run_priority() {  // Non-preemptive priority
    int time = 0;
    int completed = 0;
    int n = processes.size();

    std::vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1;
        int best_pri = 1e9;  // large number

        // Pick available process with highest priority
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival_time <= time) {
                if (processes[i].priority < best_pri) {
                    best_pri = processes[i].priority;
                    idx = i;
                }
            }
        }

        // If nothing ready, CPU idle
        if (idx == -1) {
            gantt.push_back("IDLE");
            time++;
            continue;
        }

        // First run
        if (processes[idx].start_time == -1)
            processes[idx].start_time = time;

        // Execute full burst
        for (int b = 0; b < processes[idx].burst_time; b++) {
            gantt.push_back(processes[idx].pid);
            time++;
        }

        processes[idx].completion_time = time;
        done[idx] = true;
        completed++;
    }
}

void Scheduler::run_rr(int quantum) {
    int time = 0;
    int n = processes.size();
    int completed = 0;

    // Create a queue of runnable processes
    std::queue<int> q;

    // Track remaining time
    for (auto &p : processes)
        p.remaining_time = p.burst_time;

    // To track who is in queue
    std::vector<bool> in_queue(n, false);

    // Add any process arriving at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            q.push(i);
            in_queue[i] = true;
        }
    }

    while (completed < n) {
        // No process ready → CPU idle
        if (q.empty()) {
            gantt.push_back("IDLE");
            time++;

            // Check if new processes arrived
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                    q.push(i);
                    in_queue[i] = true;
                }
            }
            continue;
        }

        int idx = q.front();
        q.pop();

        // First run timestamp
        if (processes[idx].start_time == -1)
            processes[idx].start_time = time;

        int exec = std::min(quantum, processes[idx].remaining_time);

        // Execute for 'exec' time units
        for (int t = 0; t < exec; t++) {
            gantt.push_back(processes[idx].pid);
            time++;
        }

        processes[idx].remaining_time -= exec;

        // Check arrival of new processes DURING execution
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                q.push(i);
                in_queue[i] = true;
            }
        }

        // Finished?
        if (processes[idx].remaining_time == 0) {
            processes[idx].completion_time = time;
            completed++;
        } else {
            // Not finished → back to queue
            q.push(idx);
        }
    }
}

void Scheduler::print_results() {
    print_table(processes);
    print_gantt(gantt);
    export_gantt_csv(gantt);
}

