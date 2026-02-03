#pragma once
#include <vector>
#include <string>
#include "scheduler.h"

// Print per-process statistics table
void print_table(const std::vector<Process>& processes);

// Print Gantt chart on console
void print_gantt(const std::vector<std::string>& gantt);

// Export Gantt chart to CSV
void export_gantt_csv(const std::vector<std::string>& gantt);

// NEW: Print system-level metrics
void print_system_metrics(int total_time,
                          int cpu_busy_time,
                          int process_count);
