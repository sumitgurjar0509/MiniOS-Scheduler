#include <iostream>
#include <vector>
#include <string>
#include "../include/scheduler.h"

int main() {
    std::vector<Process> processes = {
        {"P1", 0, 5, 5, 2},
        {"P2", 2, 3, 3, 1},
        {"P3", 4, 1, 1, 3}
    };

    Scheduler s1(processes, FCFS);
    s1.run();
    s1.print_results();

    return 0;
}
