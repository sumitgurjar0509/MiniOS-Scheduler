#pragma once
#include <string>
#include <vector>

struct Process {
    std::string pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int start_time = -1;
    int completion_time = 0;
};

enum Algorithm {
    FCFS,
    SJF,
    PRIORITY,
    ROUND_ROBIN
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

    // internal helpers
    void run_fcfs();
    void run_sjf();
    void run_priority();
    void run_rr();
};
