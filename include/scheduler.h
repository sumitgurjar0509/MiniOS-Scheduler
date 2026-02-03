#pragma once
#include <string>
#include <vector>

enum State {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct Process {
    std::string pid;
    int arrival_time;
    int burst_time;

    int remaining_time;
    int start_time;
    int completion_time;

    int waiting_time;
    int turnaround_time;

    int priority;

    // I/O simulation
    bool is_waiting_for_io;
    int io_remaining;

    // MLFQ / Aging
    int current_queue;
    int wait_counter;

    // Process state
    State state;
};

enum Algorithm {
    FCFS,
    SJF,
    PRIORITY,
    ROUND_ROBIN,
    MLFQ
};

class Scheduler {
public:
    Scheduler(std::vector<Process> processes,
              Algorithm algo,
              int quantum = 2,
              int contextSwitchTime = 0);

    void run();
    void print_results();

private:
    std::vector<Process> processes;
    Algorithm algo;
    int quantum;

    // Time & metrics
    int current_time;
    int cpu_busy_time;
    int context_switch_time;

    std::vector<std::string> gantt;

    // internal helpers
    void run_fcfs();
    void run_sjf();
    void run_priority();
    void run_rr(int quantum);
    void run_mlfq();

    // utilities
    void update_waiting_times();
    void handle_io();
};
