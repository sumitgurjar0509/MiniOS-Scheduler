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


void Scheduler::run_priority() {
    int time = 0;
    int completed = 0;
    int n = processes.size();

    std::vector<bool> done(n, false);

    // We assume: lower priority number = higher actual priority

    while (completed < n) {
        int idx = -1;
        int best_priority = 1e9;

        // Pick process with highest priority (smallest number)
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival_time <= time) {
                if (processes[i].priority < best_priority) {
                    best_priority = processes[i].priority;
                    idx = i;
                }
            }
        }

        // No process ready yet -> idle CPU
        if (idx == -1) {
            time++;
            continue;
        }

        // First time scheduling
        if (processes[idx].start_time == -1)
            processes[idx].start_time = time;

        for (int i = 0; i < processes[idx].burst_time; i++)
            gantt.push_back(processes[idx].pid), time++;

        processes[idx].completion_time = time;

        done[idx] = true;
        completed++;
    }
}


void Scheduler::run_rr() {
    int time = 0;
    int completed = 0;
    int n = processes.size();

    // Initialize remaining time if not already
    for (auto &p : processes)
        p.remaining_time = p.burst_time;

    std::vector<bool> added(n, false);
    std::queue<int> q;

    // Sort by arrival
    std::sort(processes.begin(), processes.end(),
              [](auto &a, auto &b){ return a.arrival_time < b.arrival_time; });

    // Push first available process
    q.push(0);
    added[0] = true;

    while (completed < n) {
        if (q.empty()) {
            time++;
            for (int i = 0; i < n; i++)
                if (!added[i] && processes[i].arrival_time <= time)
                    q.push(i), added[i] = true;
            continue;
        }

        int idx = q.front();
        q.pop();

        Process &p = processes[idx];

        if (p.start_time == -1)
            p.start_time = time;

        int exec = std::min(quantum, p.remaining_time);
        for (int i = 0; i < exec; i++)
            gantt.push_back(processes[idx].pid), time++;
        processes[idx].remaining_time -= exec;


        // Add newly arrived processes during execution
        for (int i = 0; i < n; i++) {
            if (!added[i] && processes[i].arrival_time <= time) {
                q.push(i);
                added[i] = true;
            }
        }

        if (p.remaining_time > 0) {
            q.push(idx); // back of queue
        } else {
            p.completion_time = time;
            completed++;
        }
    }
}


void Scheduler::print_results() {
    print_table(processes);
    print_gantt(gantt);
}

