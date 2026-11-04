# Systems-Lab: Linux Internals Essentials

This module forms the last stage of this suite. It focuses on low-level Linux operating system internals, virtual memory mechanisms, the `/proc` filesystem, Virtual File System (VFS) inode semantics, system call tracing (`strace`), and system daemon management via `systemd`.

---

## 1. Overview & Architecture

The `systems-lab` component provides hands-on demonstrations of core Linux operating system primitives implemented in standard C17 adhering to POSIX.1-2008 specifications.

```text
systems-lab/
├── include/
│   ├── sys_info.h       # Interfaces for /proc memory and CPU metric parsing
│   ├── mem_manager.h    # Virtual memory (mmap/mprotect) & SIGSEGV fault handler
│   └── fs_inspector.h   # VFS metadata inspection and inode/link comparison
├── src/
│   ├── sys_info.c       # Parsing /proc/meminfo and /proc/stat
│   ├── mem_manager.c    # Page allocation, permission changes, and signal recovery
│   ├── fs_inspector.c   # POSIX stat(), lstat(), hard link, and symlink demo
│   └── main.c           # Unified test suite runner
├── Makefile             # C17/POSIX build automation
└── README.md            # Comprehensive technical documentation
```

---

## 2. Experimental Modules in C

### A. Kernel Inspection via `/proc` (`sys_info.c`)
- **Concept**: User Space programs interact with the Linux kernel via pseudo-filesystems (`procfs`).
- **Implementation**:
  - `/proc/meminfo`: Reads `MemTotal`, `MemFree`, `MemAvailable`, `Buffers`, and `Cached` to calculate memory utilization.
  - `/proc/stat`: Parses CPU tick counts in user, system, idle, and iowait modes to calculate global CPU activity percentage.

### B. Virtual Memory & Protection Faults (`mem_manager.c`)
- **Concept**: The Memory Management Unit (MMU) translates virtual addresses to physical pages and enforces access permissions.
- **Implementation**:
  - Uses `mmap()` with `MAP_ANONYMOUS` to allocate a 4096-byte memory page.
  - Changes page protection from Read/Write (`PROT_READ | PROT_WRITE`) to Read-Only (`PROT_READ`) using `mprotect()`.
  - Installs a custom `SIGSEGV` handler using `sigaction()` with `SA_SIGINFO` to capture hardware page fault addresses (`info->si_addr`).
  - Demonstrates non-local control flow recovery via `sigsetjmp()` and `siglongjmp()`, avoiding process termination.
  - Releases allocated memory using `munmap()`.

### C. VFS Metadata & Inode Comparison (`fs_inspector.c`)
- **Concept**: The Virtual File System (VFS) decouples file representation from storage backends. File identity is determined by its **Inode number**, not its filename.
- **Implementation**:
  - Creates a target file and links it to a **Hard Link** (`link()`) and a **Symbolic Link** (`symlink()`).
  - Queries file metadata using `stat()` and `lstat()`.
  - **Results**:
    - **Hard Link**: Shares the exact same Inode number and increments `st_nlink` to 2.
    - **Symbolic Link**: Contains its own unique Inode number and holds a text pointer to the target path.

---

## 3. Building and Running

To build and execute the test suite:

```bash
# Navigate to module directory
cd systems-lab

# Compile project using C17 / POSIX.1-2008 standards
make

# Execute test suite
make test
```

To clean build artifacts:
```bash
make clean
```

---

## 4. System Call Analysis with `strace`

System calls (`syscalls`) bridge User Space and Kernel Space. You can trace the low-level system call execution of `systems_lab_sim` using `strace`.

### Execution Command
```bash
strace -e trace=openat,mmap,mprotect,munmap,stat,lstat,write ./systems_lab_sim
```

### Observed System Calls & Technical Descriptions
1. **`mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)`**
   - Requests a page-aligned memory allocation directly from the kernel memory manager without `brk` heap expansion.
2. **`mprotect(0x7f..., 4096, PROT_READ)`**
   - Updates page table entry (PTE) flags in kernel space, revoking write permission bit.
3. **`openat(AT_FDCWD, "/proc/meminfo", O_RDONLY)`**
   - Opens the kernel virtual filesystem entry to read RAM metrics.
4. **`stat("test_target.txt", ...)` vs `lstat("test_sym.link", ...)`**
   - `stat()` follows symbolic links and returns target file metadata. `lstat()` operates on the symbolic link itself.

---

## 5. Systemd Service Demonstration

Systemd is the standard init system and service manager for modern Linux distributions.

### Service Configuration File (`systems-lab.service`)
Create `/etc/systemd/system/systems-lab.service`:

```ini
[Unit]
Description=Systems Lab Internals Diagnostic Service
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/systems_lab_sim
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
```

### Service Management Commands
```bash
# Copy executable to binary directory
sudo cp systems_lab_sim /usr/local/bin/

# Reload systemd configuration
sudo systemctl daemon-reload

# Start the service
sudo systemctl start systems-lab.service

# Inspect service status
sudo systemctl status systems-lab.service

# View execution logs via journalctl
journalctl -u systems-lab.service --no-pager
```

---

## 6. Technical Limitations & Scope Constraints

To maintain focus on Linux system programming fundamentals, this module adheres to POSIX C17 interfaces and standard user-space Linux capabilities.

### Excluded Advanced Kernel Concepts
* **Namespaces & Control Groups:** Containerization mechanisms and resource isolation primitives (PID, NET, IPC namespaces, cgroups v1/v2) are outside the current scope.
* **Custom Kernel Modules & Drivers:** No Linux Kernel Modules (`.ko`), Device Drivers, or System Calls implementation were authored; inspection relies strictly on virtual interfaces (`/proc`).
* **Advanced Memory Isolation:** Hardware virtualization extensions (KVM, hypervisors) and complex page table manipulations beyond user-space `mmap`/`mprotect` rights are omitted.

### Environmental Constraints
* **Linux Platform Requirement:** Low-level diagnostics rely on Linux-specific virtual filesystems (`/proc/meminfo`, `/proc/stat`) and POSIX signals, making the binaries non-portable to Windows or macOS.
* **Systemd Dependency:** The system service management demonstration specifically targets Linux distributions utilizing `systemd` as PID 1 init system.