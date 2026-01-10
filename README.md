#🧠 MiniOS-Scheduler

A lightweight CPU scheduling simulator written in C++ that demonstrates how modern operating systems manage processes across different scheduling strategies.

This project implements five classic scheduling algorithms with Gantt chart visualization:
```
Algorithm	Preemptive	Priority Aware
FCFS	❌	❌
SJF (SRTF)	✔️	❌
Priority	❌	✔️
Round Robin	✔️	❌
MLFQ (Multi-Level Feedback Queue)	✔️	✔️
```
#🚀 Features
##✔ Multiple Scheduling Algorithms

Simulate and compare:

First Come First Serve (FCFS)

Shortest Job First (Preemptive SRTF)

Non-Preemptive Priority

Round Robin (RR)

Multi Level Feedback Queue (MLFQ)
With:

3 Queues (Q1→Q2→Q3)

Quantum: 2 → 4 → FCFS

Aging (prevents starvation)

Priority promotion/demotion

Optional I/O blocking simulation

Periodic priority boosting

#✔ Gantt Chart Output

Console display:
```
| P1 | P1 | P2 | P2 | P3 |
```

Plus auto export to gantt_output.csv:
```python-repl
time,process
0,P1
1,P1
2,P2
...
```
#✔ Process Statistics

For every process:

Start Time

Completion Time

Waiting Time

Turnaround Time

With averages printed automatically.

#📁 Project Structure
```makefile
MiniOS-Scheduler/
│
├── include/
│   ├── scheduler.h      # Scheduler class and enums
│   └── utils.h          # Printing + CSV export helpers
│
├── src/
│   ├── main.cpp         # Select scheduler & launch simulation
│   ├── scheduler.cpp    # All scheduling algorithms
│   └── utils.cpp        # Table print + Gantt + CSV export
│
└── build/               # Generated build folder
```
#🧪 Input Format (Inside main.cpp)

Processes are defined as:
```cpp
{
    {"P1", 0, 5, 1},
    {"P2", 2, 3, 2},
    {"P3", 4, 1, 3}
}
```

Each process has:
```
PID, Arrival Time, Burst Time, Priority (lower = higher priority)
```
#🏗 Build & Run
##📌 Requirements

CMake 3.10+

C++17 compiler (GCC, Clang, MSVC or MinGW)

##🔧 Build
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
##▶️ Run

Linux/macOS:
```bash
./scheduler
```

Windows:
```bash
.\scheduler.exe
```
#🛠 How to Switch Algorithms

Open main.cpp and change:
```cpp
Scheduler scheduler(processes, MLFQ, 2);
```

Available options:
```cpp
FCFS
SJF
PRIORITY
ROUND_ROBIN
MLFQ
```
#📈 Sample Output
```markdown
PID  AT  BT  ST  CT  WT  TAT
--------------------------------------
P1   0   5   0   5   0   5
P2   2   3   5   9   4   7
P3   4   1   8   9   4   5
--------------------------------------
Avg WT : 2.33333
Avg TAT: 5.33333

===== GANTT CHART =====
| P1 | P1 | P2 | P2 | P3 |
```
#🎯 Learning Outcomes

By building this project, you learn:

How operating systems schedule processes

Difference between preemption & non-preemption

What starvation is and how to prevent it

How multi-level feedback queues work

C++ queues, structs, loops, and system simulation logic

Reading performance metrics like Waiting/TAT

#🧩 Future Extensions

Pull requests welcome!
Ideas:

Multicore scheduling simulation

CPU & I/O pipeline (process can block and resume)

Interactive CLI input

Random process generator

GUI visualization (Qt or ImGui)

Streamlit web dashboard (Python + CSV)

#⭐ Author

##👨‍💻 Sumit Gurjar

If you find this useful, ⭐ star the repo and share!
