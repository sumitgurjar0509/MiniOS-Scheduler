#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "../include/scheduler.h"

Algorithm parseAlgorithm(const std::string& arg) {
    if (arg == "fcfs") return FCFS;
    if (arg == "sjf") return SJF;
    if (arg == "priority") return PRIORITY;
    if (arg == "rr") return ROUND_ROBIN;
    if (arg == "mlfq") return MLFQ;

    std::cerr << "Unknown algorithm: " << arg << "\n";
    std::cerr << "Usage: scheduler.exe [fcfs|sjf|priority|rr|mlfq]\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    Algorithm algo = MLFQ;  // default
    if (argc >= 2) {
        algo = parseAlgorithm(argv[1]);
    }

    std::vector<Process> processes = {
        {
            "P1", 0, 5, 5, -1, 0, 0, 0,
            2, false, 0, 1, 0, NEW
        },
        {
            "P2", 2, 3, 3, -1, 0, 0, 0,
            1, false, 0, 1, 0, NEW
        },
        {
            "P3", 4, 1, 1, -1, 0, 0, 0,
            3, false, 0, 1, 0, NEW
        }
    };

    // quantum = 2, context switch = 1
    Scheduler scheduler(processes, algo, 2, 1);
    scheduler.run();
    scheduler.print_results();

    return 0;
}
