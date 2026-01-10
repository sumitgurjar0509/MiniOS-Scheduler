#include <iostream>
#include <algorithm>
#include <queue>
#include "../include/scheduler.h"
#include "../include/utils.h"

Scheduler::Scheduler(std::vector<Process> processes, Algorithm algo, int quantum)
    : processes(processes), algo(algo), quantum(quantum) {}

void Scheduler::run() {
    for (auto &p : processes) {
    p.start_time = -1;
    p.remaining_time = p.burst_time;
    p.completion_time = 0;
    p.waiting_time = 0;
    p.turnaround_time = 0;

    p.priority = 0;
    p.is_waiting_for_io = false;
    p.io_remaining = 0;
    p.current_queue = 0;
    p.wait_counter = 0;

}
    switch (algo) {
        case FCFS: run_fcfs(); break;
        case SJF: run_sjf(); break;
        case PRIORITY: run_priority(); break;
        case ROUND_ROBIN: run_rr(); break;
        case MLFQ: run_mlfq(); break;
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

void Scheduler::run_mlfq() {
    int n = processes.size();
    int time = 0;
    int completed = 0;

    std::queue<int> q1, q2, q3;
    const int q1_quantum = 2;
    const int q2_quantum = 4;
    int boost_counter = 0;

    // Reset fields for MLFQ
    for (auto &p : processes) {
        p.remaining_time = p.burst_time;
        p.start_time = -1;
        p.completion_time = 0;
        p.waiting_time = 0;
        p.turnaround_time = 0;

        p.is_waiting_for_io = false;
        p.io_remaining = 0;
        p.current_queue = 1;
        p.wait_counter = 0;
    }

    auto admit_arrivals = [&](int t) {
        for (int i = 0; i < n; i++) {
            auto &p = processes[i];
            if (p.arrival_time <= t &&
                p.remaining_time > 0 &&
                !p.is_waiting_for_io) {

                if (p.current_queue == 1)
                    q1.push(i);
                else if (p.current_queue == 2)
                    q2.push(i);
                else
                    q3.push(i);
            }
        }
    };

    admit_arrivals(time);

    while (completed < n) {

        // === PRIORITY BOOST ===
        boost_counter++;
        if (boost_counter >= 20) {
            boost_counter = 0;

            while (!q2.empty()) { q1.push(q2.front()); q2.pop(); }
            while (!q3.empty()) { q1.push(q3.front()); q3.pop(); }

            for (auto &p : processes)
                if (p.remaining_time > 0)
                    p.current_queue = 1;
        }

        // === Pick next process ===
        int idx = -1;
        int level = -1;
        if (!q1.empty()) idx = q1.front(), q1.pop(), level = 1;
        else if (!q2.empty()) idx = q2.front(), q2.pop(), level = 2;
        else if (!q3.empty()) idx = q3.front(), q3.pop(), level = 3;
        else {
            gantt.push_back("IDLE");
            time++;
            admit_arrivals(time);
            continue;
        }

        Process &p = processes[idx];

        if (p.start_time == -1)
            p.start_time = time;

        // === RANDOM I/O SIMULATION ===
        if (!p.is_waiting_for_io && rand() % 20 == 0) {
            p.is_waiting_for_io = true;
            p.io_remaining = 3;
        }

        if (p.is_waiting_for_io) {
            gantt.push_back("IDLE");
            time++;
            p.io_remaining--;

            if (p.io_remaining == 0) {
                p.is_waiting_for_io = false;
                p.current_queue = 1;
                q1.push(idx);
            }
            admit_arrivals(time);
            continue;
        }

        // === Quantum ===
        int exec = p.remaining_time;
        if (level == 1) exec = std::min(q1_quantum, p.remaining_time);
        else if (level == 2) exec = std::min(q2_quantum, p.remaining_time);

        // === Execute ===
        for (int t = 0; t < exec; t++) {
            gantt.push_back(p.pid);
            time++;
            p.remaining_time--;

            // Aging for others
            for (int j = 0; j < n; j++) {
                if (j != idx && processes[j].remaining_time > 0 && !processes[j].is_waiting_for_io) {
                    processes[j].wait_counter++;
                    if (processes[j].wait_counter > 5) {
                        if (processes[j].current_queue > 1)
                            processes[j].current_queue--;
                        processes[j].wait_counter = 0;
                    }
                }
            }

            admit_arrivals(time);
        }

        // === Finished ===
        if (p.remaining_time == 0) {
            p.completion_time = time;
            completed++;
            continue;
        }

        // === Demote if not finished ===
        p.current_queue = std::min(p.current_queue + 1, 3);
        if (p.current_queue == 2) q2.push(idx);
        else q3.push(idx);
    }

    // === Final Stats ===
    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        if (p.waiting_time < 0) p.waiting_time = 0;
    }
}

void Scheduler::print_results() {
    print_table(processes);
    print_gantt(gantt);
    export_gantt_csv(gantt);
}

