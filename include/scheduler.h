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

/* ================= MULTI-CORE SUPPORT ================= */

struct Core {
    int id;             // Core ID
    int running_index;  // Index of process in `processes`, -1 if idle
};

enum Algorithm {
    FCFS,
    SJF,
    PRIORITY,
    ROUND_ROBIN,
    MLFQ,
    ROUND_ROBIN_MULTICORE   // NEW
};

class Scheduler {
public:
    Scheduler(std::vector<Process> processes,
              Algorithm algo,
              int quantum = 2,
              int contextSwitchTime = 0,
              int numCores = 1);   // NEW

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

    // Multi-core
    int num_cores;
    std::vector<Core> cores;

    std::vector<std::string> gantt;

    // internal helpers (single-core)
    void run_fcfs();
    void run_sjf();
    void run_priority();
    void run_rr(int quantum);
    void run_mlfq();

    // NEW: multi-core algorithm
    void run_rr_multicore();

    // utilities
    void update_waiting_times();
    void handle_io();
};
