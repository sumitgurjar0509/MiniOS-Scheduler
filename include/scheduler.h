#pragma once
#include <string>
#include <vector>

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
    bool is_waiting_for_io;   // <-- NEW
    int io_remaining;         // <-- NEW for sleep duration

    int current_queue;        // <-- Track which queue it belongs to
    int wait_counter;         // <-- For aging
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
    Scheduler(std::vector<Process> processes, Algorithm algo, int quantum = 2);

    void run();
    void print_results();

private:
    std::vector<Process> processes;
    Algorithm algo;
    int quantum;

    std::vector<std::string> gantt;

    // internal helpers
    void run_fcfs();
    void run_sjf();
    void run_priority();
    void run_rr(int quantum = 2);
    void run_mlfq();
};
