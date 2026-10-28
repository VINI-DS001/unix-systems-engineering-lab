# CPU Scheduler Simulator

A high-performance discrete-event simulator written in ISO C17 designed to model, evaluate, and compare fundamental CPU process scheduling algorithms. The simulator tracks individual process lifecycles, dynamically constructs timeline Gantt charts, and computes critical system performance metrics such as **Turnaround Time**, **Waiting Time**, **Response Time**, and **CPU Utilization**.

---

## 1. Technical Overview & Architecture

The simulator acts as an execution engine that models an operating system kernel's CPU scheduler dispatching processes from a ready queue to a single core processor. 

### Architecture & Simulation Flow

```
                     +--------------------------+
                     |  Workload Initialization |
                     | (Sample / Text File Input)|
                     +------------+-------------+
                                  |
                                  v
                     +--------------------------+
                     |  Master Process List     |
                     +------------+-------------+
                                  |
            +---------------------+---------------------+
            |                     |                     |
            v                     v                     v
     +--------------+      +--------------+      +--------------+
     | Clone List   |      | Clone List   |      | Clone List   |
     | (Run FCFS)   |      | (Run SRTF)   |      |  (Run RR)    |
     +------+-------+      +------+-------+      +------+-------+
            |                     |                     |
            +---------------------+---------------------+
                                  |
                                  v
                     +--------------------------+
                     | Execution Trace & Gantt  |
                     |     Segment Logging      |
                     +------------+-------------+
                                  |
                                  v
                     +--------------------------+
                     | Metric Computation Engine|
                     | (TAT, WT, RT, CpuUtil)   |
                     +------------+-------------+
                                  |
                                  v
                     +--------------------------+
                     | Terminal Tabular & Visual|
                     |      Chart Output        |
                     +--------------------------+
```

1. **Workload Ingestion & Deep Cloning**: The master process list is either populated with default test workloads or parsed from custom external files. Before executing each algorithm, the dataset is cloned (`clone_process_list`) to ensure isolated state mutation across runs.
2. **Discrete Time Simulation**: The main loop ticks integer time units ($t = 0, 1, 2, \dots$). At each tick, the scheduling algorithm inspects ready processes ($T_{\text{arrival}} \le t$), applies its selection criterion (e.g., shortest remaining burst, highest priority, FIFO order), and updates execution state.
3. **Gantt Segment Generation**: Consecutive CPU allocations to the same process are merged into dynamic time intervals (`gantt_segment_t`) to capture timeline visualization cleanly.
4. **Metrics Calculation**: Upon algorithm completion, process completion times are processed to compute per-process statistics and aggregate systemic averages.

---

## 2. Key Operating System Concepts & Data Structures

### Data Structures

#### Process Control Block Representation (`process_t`)
Represents an individual process state inside the simulator:
```c
typedef struct {
    int pid;             /**< Process ID */
    int arrival_time;    /**< Time process arrives in ready queue */
    int burst_time;      /**< Total CPU time required */
    int remaining_time;  /**< Remaining CPU execution time */
    int priority;        /**< Priority value (lower numerical value = higher priority) */
    int start_time;      /**< First time process gets CPU (-1 if not started) */
    int completion_time; /**< Time process completes execution */
    int turnaround_time; /**< Completion Time - Arrival Time */
    int waiting_time;    /**< Turnaround Time - Burst Time */
    int response_time;   /**< Start Time - Arrival Time */
    process_state_t state; /**< Process state (READY, RUNNING, TERMINATED) */
} process_t;
```

#### Process List Container (`process_list_t`)
Encapsulates dynamic process arrays:
```c
typedef struct {
    process_t *processes; /**< Dynamically allocated array of processes */
    int count;            /**< Total process count */
    int capacity;         /**< Maximum allocation capacity */
} process_list_t;
```

#### Gantt Timeline Segment (`gantt_segment_t`)
Stores temporal execution blocks:
```c
typedef struct {
    int pid;        /**< Process ID (-1 indicates CPU IDLE) */
    int start_time; /**< Interval start time */
    int end_time;   /**< Interval end time */
} gantt_segment_t;
```

#### Benchmark Metrics (`metrics_t`)
Captures aggregate performance benchmarks across the system workload:
```c
typedef struct {
    double avg_turnaround_time; /**< Average Turnaround Time */
    double avg_waiting_time;    /**< Average Waiting Time */
    double avg_response_time;   /**< Average Response Time */
    double cpu_utilization;     /**< CPU Utilization (%) */
    double throughput;          /**< Processes executed per time unit */
    int total_simulation_time;  /**< Final clock tick timestamp */
    int total_idle_time;        /**< Total ticks CPU was idle */
} metrics_t;
```

---

## 3. Repository & File Structure

```
scheduler/
├── Makefile                # Build automation script (GCC / C17)
├── README.md               # Architecture documentation and manual
├── include/
│   ├── metrics.h           # Header for metric calculation and output formatting
│   ├── process.h           # Header for process structures and memory management
│   └── scheduler.h         # Header for scheduling algorithms and Gantt structures
└── src/
    ├── algorithms.c        # Implementation of FCFS, SJF, SRTF, RR, Priority algorithms
    ├── main.c              # Application entry point and workload loader
    ├── metrics.c           # Metric evaluation and terminal printing logic
    └── process.c           # Allocation, deep cloning, and reset utilities
```

---

## 4. Compilation & Execution Instructions

### Prerequisites
* **Compiler**: GCC or Clang supporting C17.
* **Build Tool**: GNU Make.
* **Environment**: POSIX-compliant system (Linux, macOS, WSL).

### Build Target
Build the project using the root or module Makefile:
```bash
make clean && make
```
This produces the binary target `scheduler_sim`.

### Running the Simulator

#### 1. Default Benchmark Workload
Run without arguments to execute the built-in 5-process benchmark:
```bash
./scheduler_sim
```

#### 2. Custom Dataset Input File
Pass a plain text file path containing process specifications:
```bash
./scheduler_sim custom_workload.txt
```

##### Input File Format
The input file expects one process definition per line in space-separated format:
```text
<PID> <Arrival_Time> <Burst_Time> <Priority>
```

##### Example Dataset (`workload.txt`)
```text
1 0 8 3
2 1 4 1
3 2 9 4
4 3 5 2
5 4 2 5
```

---

## 5. Implemented Algorithms & Usage

| Algorithm | Type | Description / Selection Logic |
| :--- | :--- | :--- |
| **First-Come, First-Served (FCFS)** | Non-preemptive | Dispatches ready processes strictly in order of arrival time ($T_{\text{arrival}}$). |
| **Shortest Job First (SJF)** | Non-preemptive | Selects the ready process with the smallest total burst time ($T_{\text{burst}}$). Runs to completion. |
| **Shortest Remaining Time First (SRTF)** | Preemptive | Preempts execution if a newly arrived process has a remaining burst time ($T_{\text{remaining}}$) strictly less than the currently running process. |
| **Round Robin (RR)** | Preemptive | Allocates CPU time slices (Time Quantum $Q = 2$) via a FIFO circular ready queue. |
| **Priority (Non-Preemptive)** | Non-preemptive | Selects ready process with the highest priority (lowest numeric value). Runs to completion. |
| **Priority (Preemptive)** | Preemptive | Re-evaluates priority on every clock tick. Preempts if a ready process has higher priority than the running process. |

---

## 6. Metrics & Benchmark Results

### Mathematical Formulas

* **Turnaround Time ($TAT$)**: Total elapsed time from process arrival to execution completion.
  $$TAT = T_{\text{completion}} - T_{\text{arrival}}$$
* **Waiting Time ($WT$)**: Total time Spent waiting in the ready queue.
  $$WT = TAT - T_{\text{burst}}$$
* **Response Time ($RT$)**: Time elapsed from arrival until initial CPU allocation.
  $$RT = T_{\text{start}} - T_{\text{arrival}}$$
* **CPU Utilization ($\text{Util}$)**: Percentage of simulation time spent actively executing processes.
  $$\text{Util} = \left( \frac{T_{\text{simulation}} - T_{\text{idle}}}{T_{\text{simulation}}} \right) \times 100\%$$
* **Throughput**: Rate of process completion per unit of time.
  $$\text{Throughput} = \frac{N_{\text{processes}}}{T_{\text{simulation}}}$$

---

### Comparative Benchmark Output

The baseline test set consists of the following 5 processes:
* $P_1$: $T_{\text{arr}} = 0, T_{\text{burst}} = 8, \text{Prio} = 3$
* $P_2$: $T_{\text{arr}} = 1, T_{\text{burst}} = 4, \text{Prio} = 1$
* $P_3$: $T_{\text{arr}} = 2, T_{\text{burst}} = 9, \text{Prio} = 4$
* $P_4$: $T_{\text{arr}} = 3, T_{\text{burst}} = 5, \text{Prio} = 2$
* $P_5$: $T_{\text{arr}} = 4, T_{\text{burst}} = 2, \text{Prio} = 5$

#### Execution Summary Matrix

| Algorithm | Total Time | Avg TAT | Avg WT | Avg RT | CPU Util (%) | Throughput |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **FCFS** | 28 | 15.60 | 10.00 | 10.00 | 100.0% | 0.1786 |
| **SJF (Non-preemptive)** | 28 | 11.80 | 6.20 | 6.20 | 100.0% | 0.1786 |
| **SRTF (Preemptive)** | 28 | 10.40 | 4.80 | 3.00 | 100.0% | 0.1786 |
| **Round Robin ($Q=2$)** | 28 | 15.40 | 9.80 | 3.20 | 100.0% | 0.1786 |
| **Priority (Non-preemptive)** | 28 | 12.00 | 6.40 | 6.40 | 100.0% | 0.1786 |
| **Priority (Preemptive)** | 28 | 12.00 | 6.40 | 3.80 | 100.0% | 0.1786 |

---

## 7. Technical Limitations & Scope Constraints

1. **Single Core Processor Modeling**: The engine models single-core hardware setups without concurrent multi-core dispatch queues.
2. **Zero Context-Switch Overhead**: Idealized simulation where context switches occur instantaneously ($0$ overhead cost).
3. **CPU-Bound Workload Assumption**: Does not simulate I/O burst intermingling (e.g., disk reads, socket waits) or multi-threading blocking states (`BLOCKED`).
4. **Static Workload Capacity**: Fixed allocation caps maximum active processes per dataset to 100 (`MAX_PROCESSES`).