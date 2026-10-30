/**
 * @file main.c
 * @brief Entry point for the Systems Lab Linux Internals demonstration tool.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/sys_info.h"

int main(void) {
    printf("=======================================================\n");
    printf("     LINUX INTERNALS ESSENTIALS (C17 / POSIX)         \n");
    printf("=======================================================\n");

    /* Test sub-module 1: /proc Virtual Filesystem Diagnostics */
    print_system_diagnostics();

    return EXIT_SUCCESS;
}