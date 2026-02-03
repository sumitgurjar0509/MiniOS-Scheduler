#include <iostream>
#include <fstream>
#include <iomanip>
#include "../include/utils.h"

void print_table(const std::vector<Process>& processes) {
    double total_wt = 0, total_tat = 0;

    std::cout << "\nPID  AT  BT  ST  CT  WT  TAT\n";
    std::cout << "--------------------------------------\n";

    for (const auto& p : processes) {
        int tat = p.completion_time - p.arrival_time;
        int wt  = tat - p.burst_time;
        if (wt < 0) wt = 0;

        total_wt += wt;
        total_tat += tat;

        std::cout << p.pid << "   "
                  << p.arrival_time << "   "
                  << p.burst_time << "   "
                  << p.start_time << "   "
                  << p.completion_time << "   "
                  << wt << "   "
                  << tat << "\n";
    }

    std::cout << "--------------------------------------\n";
    std::cout << "Avg WT : " << total_wt / processes.size() << "\n";
    std::cout << "Avg TAT: " << total_tat / processes.size() << "\n";
}

void print_gantt(const std::vector<std::string>& gantt) {
    std::cout << "\n===== GANTT CHART =====\n";
    for (size_t t = 0; t < gantt.size(); t++)
        std::cout << "T=" << t << " | " << gantt[t] << "\n";
}

void export_gantt_csv(const std::vector<std::string>& gantt) {
    std::ofstream file("gantt_output.csv");
    file << "time,execution\n";
    for (size_t t = 0; t < gantt.size(); t++)
        file << t << "," << gantt[t] << "\n";
    std::cout << "Gantt CSV exported -> gantt_output.csv\n";
}

void print_system_metrics(int total_time,
                          int cpu_busy_time,
                          int process_count) {
    double cpu_util = (total_time > 0)
        ? (double(cpu_busy_time) / total_time) * 100.0
        : 0.0;

    double throughput = (total_time > 0)
        ? (double(process_count) / total_time)
        : 0.0;

    std::cout << "\n===== SYSTEM METRICS =====\n";
    std::cout << "Total Time     : " << total_time << "\n";
    std::cout << "CPU Busy Time  : " << cpu_busy_time << "\n";
    std::cout << "CPU Utilization: " << cpu_util << "%\n";
    std::cout << "Throughput     : " << throughput << "\n";
}

void export_performance_report(const std::string& algorithm,
                               int cores,
                               double avg_wt,
                               double avg_tat,
                               double cpu_util,
                               double throughput) {
    std::ofstream file("performance_report.csv", std::ios::app);

    if (file.tellp() == 0) {
        file << "algorithm,cores,avg_waiting_time,avg_turnaround_time,"
             << "cpu_utilization,throughput\n";
    }

    file << algorithm << ","
         << cores << ","
         << avg_wt << ","
         << avg_tat << ","
         << cpu_util << ","
         << throughput << "\n";

    std::cout << "Performance data appended -> performance_report.csv\n";
}
