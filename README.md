# 🧠 MiniOS Scheduler

A C++ CPU Scheduling Simulator that models classical and modern operating system scheduling algorithms, supports multi-core execution, generates performance metrics, and enables experimental comparison using automated reports and visualizations.

## 🚀 Features

✅ Multiple scheduling algorithms:

FCFS (First Come First Serve)

SJF (Preemptive – Shortest Remaining Time First)

Priority Scheduling

Round Robin (RR)

Multi-Level Feedback Queue (MLFQ)

Multi-Core Round Robin (RR-MC)

✅ Runtime configuration via command-line arguments

✅ File-based workload input

✅ Multi-core CPU simulation

✅ Gantt chart visualization (console + CSV)

✅ System-level metrics:

Average Waiting Time

Average Turnaround Time

CPU Utilization

Throughput

✅ Automated performance comparison report

✅ Python-based plotting for experiments

## 🏗️ Project Structure
```
MiniOS-Scheduler/
├── include/
│   ├── scheduler.h
│   └── utils.h
├── src/
│   ├── scheduler.cpp
│   ├── utils.cpp
│   └── main.cpp
├── build/                # build artifacts (ignored by git)
├── input.txt             # sample workload
├── plot_results.py       # Python plotting script
├── performance_report.csv
├── CMakeLists.txt
└── README.md
```
## 🛠️ Build Instructions
### Prerequisites

C++17 compiler (GCC / MinGW / MSVC)

CMake ≥ 3.10

Python 3 (for plotting)

### Build
```
cmake -S . -B build
cmake --build build
```
## ▶️ How to Run
### General Format
```
scheduler.exe <algorithm> <input_file> [num_cores]
```
### Examples
#### Single-core Round Robin
```
scheduler.exe rr input.txt
```
#### Multi-core Round Robin (2 cores)
```
scheduler.exe rr-mc input.txt 2
```
#### Default (MLFQ, single core)
```
scheduler.exe
```
### 📄 Input File Format

#### input.txt
```
PID AT BT PRIORITY
P1 0 5 2
P2 2 3 1
P3 4 1 3
```

Where:

AT = Arrival Time

BT = Burst Time

## 📊 Performance Metrics

For every run, the scheduler computes:

Average Waiting Time

Average Turnaround Time

CPU Utilization

Throughput

All results are automatically appended to:
```
performance_report.csv
```
## 🧪 Experimental Evaluation
### Methodology

Same workload executed using different scheduling algorithms

Metrics collected programmatically

Results exported as CSV

Visualization performed using Python

### Example CSV Output
```
algorithm,cores,avg_waiting_time,avg_turnaround_time,cpu_utilization,throughput
fcfs,1,2.33,5.33,100.0,0.33
rr,1,3.00,6.00,95.0,0.30
rr-mc,2,1.50,4.20,180.0,0.66
```

⚠️ CPU utilization can exceed 100% in multi-core systems since it represents aggregate core usage.

## 📈 Visualization (Python)

Generate comparison plots:
```
python plot_results.py
```

Generated graphs:

Average Waiting Time

Average Turnaround Time

CPU Utilization

Throughput

Saved as .png files for reports or papers.

## 🎓 Learning Outcomes

This project demonstrates:

Practical understanding of OS scheduling policies

Differences between preemptive and non-preemptive scheduling

Effects of time quantum and core count

Realistic multi-core scheduling behavior

Experimental performance evaluation techniques

## 🧩 Future Extensions

Multi-core MLFQ

Priority aging toggle

I/O blocking simulation

Linux CFS-inspired scheduler

Web-based visualization dashboard

## 👤 Author

### Sumit Gurjar
Operating Systems | C++ | Systems Programming

## ⭐ If you find this useful

Give the repository a ⭐ — it helps a lot!
