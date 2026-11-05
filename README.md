# Unix Systems Engineering Lab

[![Language: C17](https://img.shields.io/badge/Language-C17-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C17_(C_standard_revision))
[![Standard: POSIX.1-2008](https://img.shields.io/badge/Standard-POSIX.1--2008-FCC624?style=flat&logo=linux&logoColor=black)](https://pubs.opengroup.org/onlinepubs/9699919799/)
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-FCC624?style=flat&logo=linux&logoColor=black)](https://www.kernel.org/)
[![Compiler: GCC](https://img.shields.io/badge/Compiler-GCC-A42E2B?style=flat&logo=gnu&logoColor=white)](https://gcc.gnu.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A hands-on collection of low-level systems programming projects implemented in **C17** adhering to **POSIX.1-2008** standards on Linux platforms. 

This repository demonstrates fundamental concepts of Operating System engineering including process lifecycle management, IPC mechanisms, CPU scheduling algorithm simulation, Virtual Memory (`mmap`/`mprotect`), POSIX signals, VFS metadata tracking, and `systemd` daemon management.

---

## 1. Project Architecture & Modules

The repository is structured into three self-contained experimental stages:

```
.
├── shell/          # Stage 1: Custom POSIX Command Line Interpreter
│   ├── include/         # Built-in commands & process control headers
│   ├── src/             # Shell loop, fork/execve, pipes, and signal logic
│   ├── Makefile         # Build configuration
│   └── README.md        # Technical specification for Stage 1
│
├── scheduler/       # Stage 2: CPU Scheduling Algorithm Simulator
│   ├── include/         # Process Control Block (PCB) & scheduler headers
│   ├── src/             # FCFS, SJF (Non-preemptive), and Round-Robin engines
│   ├── Makefile         # Build configuration
│   └── README.md        # Technical specification for Stage 2
│
├── systems-lab/         # Stage 3: Linux Internals Essentials
│   ├── include/         # /proc parsing, Virtual Memory & VFS headers
│   ├── src/             # procfs stats, mmap/SIGSEGV protection, and inode demo
│   ├── Makefile         # Build configuration
│   └── README.md        # Technical specification for Stage 3
|
├── Makefile             # Root build script orchestrating all sub-modules
└── README.md            # Repository root documentation (this file)
```

---

## 2. Executive Summary of Stages

### Stage 1: Mini-Shell (`shell/`)
* **Objective**: Implement a POSIX-compliant interactive shell.
* **Key Features**:
  * Process creation and replacement via `fork()` and `execvp()`.
  * Input/Output redirection (`>`, `<`) using `dup2()`.
  * Inter-Process Communication (IPC) pipelines (`|`).
  * Built-in commands (`cd`, `help`, `exit`) running in the shell's execution context.
  * Robust signal handling (`SIGINT` / `SIGTSTP`) preventing shell termination.

### Stage 2: CPU Scheduler Simulator (`scheduler/`)
* **Objective**: Simulate core OS process scheduling algorithms and evaluate efficiency metrics.
* **Algorithms Implemented**:
  * **FCFS** (First-Come, First-Served): Non-preemptive baseline FIFO scheduling.
  * **SJF** (Shortest Job First): Non-preemptive shortest CPU burst prioritization.
  * **Round-Robin (RR)**: Preemptive time-sliced CPU allocation with configurable quantum length.
* **Metrics Computed**: Turnaround Time, Waiting Time, Response Time, and Gantt chart output visualization.

### Stage 3: Linux Internals Essentials (`systems-lab/`)
* **Objective**: Perform user-space C experiments demonstrating kernel interfaces and OS mechanics.
* **Core Demonstrations**:
  * **Kernel Inspection (`/proc`)**: Parse `/proc/meminfo` and `/proc/stat` to calculate live RAM and CPU utilization percentages.
  * **Virtual Memory & Page Faults**: Dynamic page allocation via `mmap()`, memory protection restrictions using `mprotect()`, and `SIGSEGV` signal recovery via `sigaction()` and `siglongjmp()`.
  * **VFS & Inode Inspection**: Metadata comparison between original files, Hard Links, and Symbolic Links using `stat()` and `lstat()`.
  * **Diagnostics & System Services**: System call tracing via `strace` and execution management via `systemd` oneshot unit services.

---

## 3. Environment & Prerequisites

* **Operating System**: Linux (Ubuntu 20.04+, Debian 11+, Fedora 34+ or Linux VM).
* **Compiler**: GCC with C17 standard support (`-std=c17`).
* **Build System**: GNU Make (`make`).
* **System Utilities**: `strace`, `systemd` / `systemctl` (for Stage 3 service demonstration).

---

## 4. Quick Start Guide

### Cloning the Repository
```bash
git clone https://github.com/VINI-DS001/unix-systems-engineering-lab.git
cd unix-systems-engineering-lab
```

### Building & Running Individual Modules

#### 1. Mini-Shell
```bash
cd shell
make
make test        # Launches interactive shell session
```

#### 2. CPU Scheduler Simulator
```bash
cd ../scheduler
make
make test        # Runs FCFS, SJF, and Round-Robin simulation test cases
```

#### 3. Linux Internals Essentials
```bash
cd ../systems-lab
make
make test        # Executes /proc diagnostic, mmap/SIGSEGV fault, and inode tests
```

---

## 5. Scope Constraints & Limitations

To maintain a clear educational focus on user-space POSIX systems programming, the following elements are explicitly out of scope:
* Kernel-space code, Kernel Modules (`.ko`), and Custom Device Drivers.
* Advanced containerization primitives (Namespaces, cgroups v1/v2).
* Hardware virtualization extensions (KVM, hypervisors).

---

## 6. License

This repository is maintained for educational and systems engineering research purposes. All of the source code adheres to POSIX standards and architectural principles.