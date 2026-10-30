/**
 * @file mem_manager.c
 * @brief Virtual memory allocation, mprotect execution, and SIGSEGV signal handler implementation.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>
#include "../include/mem_manager.h"

static sigjmp_buf jump_buffer;
static volatile sig_atomic_t segv_caught = 0;

/**
 * @brief Signal handler for SIGSEGV (Segmentation Fault / Page Fault Violation).
 */
static void segv_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;
    segv_caught = 1;
    printf("\n[SIGSEGV HANDLER] Intercepted Segmentation Fault!\n");
    printf("  Faulting Memory Address: %p\n", info->si_addr);
    printf("  Reason: Memory protection violation (mprotect PROT_READ).\n");
    
    /* Safely jump out of the faulting instruction */
    siglongjmp(jump_buffer, 1);
}

int run_virtual_memory_demo(void) {
    printf("\n=======================================================\n");
    printf("   VIRTUAL MEMORY & PROTECTION FAULT EXPERIMENT        \n");
    printf("=======================================================\n");

    long page_size = sysconf(_SC_PAGESIZE);
    printf("System Memory Page Size: %ld bytes\n", page_size);

    /* 1. Allocate 1 anonymous memory page via mmap */
    char *ptr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return -1;
    }

    printf("1. Allocated page via mmap() at address: %p\n", (void *)ptr);

    /* Write to page while RW */
    strcpy(ptr, "Systems Engineering Lab - POSIX C17");
    printf("   Written data: \"%s\"\n", ptr);

    /* 2. Setup SIGSEGV signal handler */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = segv_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction failed");
        munmap(ptr, page_size);
        return -1;
    }

    /* 3. Change page permission to READ-ONLY via mprotect */
    printf("2. Restricting permissions to PROT_READ using mprotect()...\n");
    if (mprotect(ptr, page_size, PROT_READ) == -1) {
        perror("mprotect failed");
        munmap(ptr, page_size);
        return -1;
    }

    /* 4. Trigger controlled write fault */
    printf("3. Attempting write to READ-ONLY memory to provoke SIGSEGV...\n");

    if (sigsetjmp(jump_buffer, 1) == 0) {
        /* This line triggers write fault on PROT_READ page */
        ptr[0] = 'X'; 
    } else {
        printf("4. Successfully recovered from SIGSEGV handler using siglongjmp()!\n");
    }

    /* 5. Clean up memory */
    printf("5. Deallocating memory page via munmap()...\n");
    if (munmap(ptr, page_size) == -1) {
        perror("munmap failed");
        return -1;
    }

    printf("Virtual Memory Experiment Completed Successfully.\n");
    printf("=======================================================\n");

    return 0;
}