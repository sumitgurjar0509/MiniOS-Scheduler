#include <iostream>
#include <fstream>
#include <iomanip>
#include "../include/utils.h"

void print_table(const std::vector<Process> &processes) {
    std::cout << "\nPID  AT  BT  ST  CT  WT  TAT\n";
    std::cout << "--------------------------------------\n";

    double total_wt = 0, total_tat = 0;

    for (const auto &p : processes) {
        int tat = p.completion_time - p.arrival_time;
        int wt  = tat - p.burst_time;
        if (wt < 0) wt = 0;

        total_wt += wt;
        total_tat += tat;

        std::cout << std::setw(3) << p.pid << "  "
                  << std::setw(2) << p.arrival_time << "  "
                  << std::setw(2) << p.burst_time << "  "
                  << std::setw(2) << p.start_time << "  "
                  << std::setw(2) << p.completion_time << "  "
                  << std::setw(2) << wt << "  "
                  << std::setw(3) << tat << "\n";
    }

    std::cout << "--------------------------------------\n";
    std::cout << "Avg WT : " << total_wt / processes.size() << "\n";
    std::cout << "Avg TAT: " << total_tat / processes.size() << "\n";
}

void print_gantt(const std::vector<std::string> &gantt) {
    std::cout << "\n===== GANTT CHART =====\n";

    for (size_t t = 0; t < gantt.size(); t++) {
        std::cout << "T=" << std::setw(2) << t << " | "
                  << gantt[t] << "\n";
    }
}

void export_gantt_csv(const std::vector<std::string> &gantt) {
    std::ofstream file("gantt_output.csv");
    file << "time,execution\n";
    for (size_t t = 0; t < gantt.size(); t++)
        file << t << "," << gantt[t] << "\n";

    std::cout << "Gantt CSV exported -> gantt_output.csv\n";
}

void print_system_metrics(int total_time,
                          int cpu_busy_time,
                          int process_count) {
    double cpu_util = total_time > 0
        ? (double(cpu_busy_time) / total_time) * 100.0
        : 0.0;

    std::cout << "\n===== SYSTEM METRICS =====\n";
    std::cout << "Total Time     : " << total_time << "\n";
    std::cout << "CPU Busy Time  : " << cpu_busy_time << "\n";
    std::cout << "CPU Utilization: " << std::fixed << std::setprecision(2)
              << cpu_util << "%\n";
    std::cout << "Throughput     : "
              << (double(process_count) / total_time)
              << " processes/unit time\n";
}
