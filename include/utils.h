#pragma once
#include <vector>
#include "scheduler.h"

void print_table(const std::vector<Process> &processes);
void print_gantt(const std::vector<std::string> &gantt);
void export_gantt_csv(const std::vector<std::string> &gantt);
