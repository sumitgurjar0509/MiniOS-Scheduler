#pragma once
#include <vector>
#include <string>
#include "scheduler.h"

void print_table(const std::vector<Process>& processes);
void print_gantt(const std::vector<std::string>& gantt);
void export_gantt_csv(const std::vector<std::string>& gantt);

// System metrics
void print_system_metrics(int total_time,
                          int cpu_busy_time,
                          int process_count);

// NEW: Performance report export
void export_performance_report(const std::string& algorithm,
                               int cores,
                               double avg_wt,
                               double avg_tat,
                               double cpu_util,
                               double throughput);
