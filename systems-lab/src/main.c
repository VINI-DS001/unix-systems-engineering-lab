/**
 * @file main.c
 * @brief Entry point for the Systems Lab Linux Internals demonstration tool.
 *
 * Orchestrates discrete experimental demonstrations covering Linux low-level concepts:
 * 1. Kernel procfs parsing (/proc/meminfo and /proc/stat).
 * 2. Virtual memory allocation (mmap), page protection (mprotect), and fault interception (SIGSEGV).
 * 3. Virtual File System (VFS) metadata and inode comparison (stat, lstat, hard/symbolic links).
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/sys_info.h"
#include "../include/mem_manager.h"
#include "../include/fs_inspector.h"

/**
 * @brief Main execution function for the Linux Internals Essentials suite.
 *
 * Calls each experimental sub-module sequentially and returns success status.
 *
 * @return EXIT_SUCCESS on completion.
 */
int main(void) {
    printf("=======================================================\n");
    printf("     LINUX INTERNALS ESSENTIALS (C17 / POSIX)         \n");
    printf("=======================================================\n");

    /* Test 1: /proc Virtual Filesystem Diagnostics */
    print_system_diagnostics();

    /* Test 2: Virtual Memory & mprotect / SIGSEGV Demo */
    run_virtual_memory_demo();

    /* Test 3: Filesystem, Inodes & Link Inspection Demo */
    run_filesystem_demo();

    return EXIT_SUCCESS;
}