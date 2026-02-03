#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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
    std::cerr << "Usage: scheduler.exe [fcfs|sjf|priority|rr|mlfq] [input.txt]\n";
    exit(1);
}

std::vector<Process> loadProcessesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Process> processes;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file: " << filename << "\n";
        return processes;
    }

    std::string header;
    std::getline(file, header); // skip header

    Process p;
    while (file >> p.pid >> p.arrival_time >> p.burst_time >> p.priority) {
        p.remaining_time = p.burst_time;
        p.start_time = -1;
        p.completion_time = 0;
        p.waiting_time = 0;
        p.turnaround_time = 0;
        p.is_waiting_for_io = false;
        p.io_remaining = 0;
        p.current_queue = 1;
        p.wait_counter = 0;
        p.state = NEW;
        processes.push_back(p);
    }

    return processes;
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    Algorithm algo = MLFQ;
    std::string inputFile;

    if (argc >= 2)
        algo = parseAlgorithm(argv[1]);

    if (argc >= 3)
        inputFile = argv[2];

    std::vector<Process> processes;

    if (!inputFile.empty())
        processes = loadProcessesFromFile(inputFile);

    // Fallback (default workload)
    if (processes.empty()) {
        processes = {
            {"P1", 0, 5, 5, -1, 0, 0, 0, 2, false, 0, 1, 0, NEW},
            {"P2", 2, 3, 3, -1, 0, 0, 0, 1, false, 0, 1, 0, NEW},
            {"P3", 4, 1, 1, -1, 0, 0, 0, 3, false, 0, 1, 0, NEW}
        };
    }

    Scheduler scheduler(processes, algo, 2, 1);
    scheduler.run();
    scheduler.print_results();

    return 0;
}
