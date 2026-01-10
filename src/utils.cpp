#include <iostream>
#include <iomanip>
#include "../include/utils.h"

void print_table(const std::vector<Process> &processes) {
    std::cout << "\nPID  AT  BT  ST  CT  WT  TAT\n";
    std::cout << "--------------------------------------\n";

    double total_wt = 0, total_tat = 0;

    for (auto &p : processes) {
        int tat = p.completion_time - p.arrival_time;
        int wt  = tat - p.burst_time;
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

std::string color(const std::string &pid) {
    int hash = 0;
    for (char c : pid) hash = (hash + c) % 6;
    switch (hash) {
        case 0: return "\033[31m" + pid + "\033[0m";  // Red
        case 1: return "\033[32m" + pid + "\033[0m";  // Green
        case 2: return "\033[33m" + pid + "\033[0m";  // Yellow
        case 3: return "\033[34m" + pid + "\033[0m";  // Blue
        case 4: return "\033[35m" + pid + "\033[0m";  // Magenta
        default: return "\033[36m" + pid + "\033[0m"; // Cyan
    }
}

void print_gantt(const std::vector<std::string> &gantt) {
    std::cout << "\n===== GANTT CHART =====\n";

    // Top bar
    std::cout << " ";
    for (auto &p : gantt) std::cout << "----";
    std::cout << "-\n|";

    // Process labels
    for (auto &p : gantt) std::cout << " " << color(p) << " |";
    std::cout << "\n ";

    // Bottom bar
    for (auto &p : gantt) std::cout << "----";
    std::cout << "-\n";

    // Time markers
    int t = 0;
    std::cout << t;
    for (int i = 0; i < gantt.size(); i++) {
        if ((i+1) >= 10) std::cout << "   " << i+1;
        else std::cout << "    " << i+1;
    }
    std::cout << "\n";
}
void export_gantt_csv(const std::vector<std::string> &gantt) {
    std::ofstream file("gantt_output.csv");
    file << "time,pid\n";
    for (int t = 0; t < gantt.size(); t++) {
        file << t << "," << gantt[t] << "\n";
    }
    file.close();
    std::cout << "Gantt CSV exported -> gantt_output.csv\n";
}

