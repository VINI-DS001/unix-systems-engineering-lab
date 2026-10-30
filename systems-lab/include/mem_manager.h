/**
 * @file mem_manager.h
 * @brief Interfaces for virtual memory management, mprotect, and SIGSEGV handling.
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