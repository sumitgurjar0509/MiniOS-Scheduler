#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "../include/scheduler.h"

/* ================= ALGORITHM PARSER ================= */

Algorithm parseAlgorithm(const std::string& arg) {
    if (arg == "fcfs")   return FCFS;
    if (arg == "sjf")    return SJF;
    if (arg == "priority") return PRIORITY;
    if (arg == "rr")     return ROUND_ROBIN;
    if (arg == "mlfq")   return MLFQ;
    if (arg == "rr-mc")  return ROUND_ROBIN_MULTICORE;

    std::cerr << "Unknown algorithm: " << arg << "\n";
    std::cerr << "Usage:\n";
    std::cerr << "  scheduler.exe <algo> [input.txt] [num_cores]\n";
    std::cerr << "Algorithms:\n";
    std::cerr << "  fcfs | sjf | priority | rr | mlfq | rr-mc\n";
    exit(1);
}

/* ================= FILE INPUT ================= */

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

/* ================= MAIN ================= */

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    Algorithm algo = MLFQ;
    std::string inputFile;
    int numCores = 1;

    if (argc >= 2)
        algo = parseAlgorithm(argv[1]);

    if (argc >= 3)
        inputFile = argv[2];

    if (argc >= 4)
        numCores = std::max(1, std::atoi(argv[3]));

    std::vector<Process> processes;

    if (!inputFile.empty())
        processes = loadProcessesFromFile(inputFile);

    /* Fallback workload */
    if (processes.empty()) {
        processes = {
            {"P1", 0, 5, 5, -1, 0, 0, 0, 2, false, 0, 1, 0, NEW},
            {"P2", 2, 3, 3, -1, 0, 0, 0, 1, false, 0, 1, 0, NEW},
            {"P3", 4, 1, 1, -1, 0, 0, 0, 3, false, 0, 1, 0, NEW}
        };
    }

    /* quantum = 2, context switch = 1 */
    Scheduler scheduler(processes, algo, 2, 1, numCores);
    scheduler.run();
    scheduler.print_results();

    return 0;
}
