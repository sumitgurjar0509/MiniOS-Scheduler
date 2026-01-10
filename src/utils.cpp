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
