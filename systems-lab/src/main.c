/**
 * @file main.c
 * @brief Entry point for the Systems Lab Linux Internals demonstration tool.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/sys_info.h"
#include "../include/mem_manager.h"
#include "../include/fs_inspector.h"

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