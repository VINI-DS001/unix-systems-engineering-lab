/**
 * @file mem_manager.h
 * @brief Interfaces for virtual memory management, mprotect, and SIGSEGV handling.
 *
 * Provides functions for low-level virtual memory manipulation using POSIX APIs (mmap, mprotect, munmap).
 * Demonstrates hardware page fault mechanics, memory protection enforcement (PROT_READ vs PROT_WRITE),
 * signal handling for SIGSEGV via sigaction(), and controlled execution flow restoration using siglongjmp().
 */

#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>

/**
 * @brief Demonstrates dynamic page allocation using mmap(), permission changes
 *        via mprotect(), and catching page faults / SIGSEGV signals.
 * 
 * @return 0 on successful demonstration, -1 on error.
 */
int run_virtual_memory_demo(void);

#endif /* MEM_MANAGER_H */