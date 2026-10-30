/**
 * @file sys_info.c
 * @brief Implementation of Linux /proc virtual filesystem inspection functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sys_info.h"

int get_system_memory_info(sys_mem_info_t *info) {
    if (!info) return -1;

    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("systems-lab: error opening /proc/meminfo");
        return -1;
    }

    char line[256];
    int parsed_count = 0;

    memset(info, 0, sizeof(sys_mem_info_t));

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &info->total_ram_kb) == 1) parsed_count++;
        else if (sscanf(line, "MemFree: %lu kB", &info->free_ram_kb) == 1) parsed_count++;
        else if (sscanf(line, "MemAvailable: %lu kB", &info->avail_ram_kb) == 1) parsed_count++;
        else if (sscanf(line, "Buffers: %lu kB", &info->buffers_kb) == 1) parsed_count++;
        else if (sscanf(line, "Cached: %lu kB", &info->cached_kb) == 1) parsed_count++;

        if (parsed_count >= 5) break;
    }

    fclose(fp);
    return (parsed_count > 0) ? 0 : -1;
}

int get_system_cpu_info(sys_cpu_info_t *info) {
    if (!info) return -1;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("systems-lab: error opening /proc/stat");
        return -1;
    }

    char line[256];
    memset(info, 0, sizeof(sys_cpu_info_t));

    if (fgets(line, sizeof(line), fp)) {
        /* Line format: cpu  user nice system idle iowait irq softirq steal guest guest_nice */
        if (strncmp(line, "cpu ", 4) == 0) {
            sscanf(line + 4, "%llu %llu %llu %llu %llu",
                   &info->user, &info->nice, &info->system,
                   &info->idle, &info->iowait);
        }
    }

    fclose(fp);
    return 0;
}

void print_system_diagnostics(void) {
    sys_mem_info_t mem;
    sys_cpu_info_t cpu;

    printf("\n=======================================================\n");
    printf("         LINUX KERNEL INTERNALS DIAGNOSTIC            \n");
    printf("=======================================================\n");

    if (get_system_memory_info(&mem) == 0) {
        double used_ram = (double)(mem.total_ram_kb - mem.avail_ram_kb) / 1024.0;
        double total_ram = (double)mem.total_ram_kb / 1024.0;
        double usage_pct = (used_ram / total_ram) * 100.0;

        printf("[Memory Statistics (/proc/meminfo)]\n");
        printf("  Total RAM     : %.2f MB (%lu kB)\n", total_ram, mem.total_ram_kb);
        printf("  Free RAM      : %.2f MB (%lu kB)\n", (double)mem.free_ram_kb / 1024.0, mem.free_ram_kb);
        printf("  Available RAM : %.2f MB (%lu kB)\n", (double)mem.avail_ram_kb / 1024.0, mem.avail_ram_kb);
        printf("  Buffers/Cache : %.2f MB (%lu kB)\n", (double)(mem.buffers_kb + mem.cached_kb) / 1024.0, mem.buffers_kb + mem.cached_kb);
        printf("  RAM Utilization: %.2f%%\n", usage_pct);
    } else {
        printf("Failed to retrieve memory statistics.\n");
    }

    printf("-------------------------------------------------------\n");

    if (get_system_cpu_info(&cpu) == 0) {
        unsigned long long total_ticks = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait;
        unsigned long long active_ticks = cpu.user + cpu.nice + cpu.system;

        printf("[CPU Ticks Counter (/proc/stat)]\n");
        printf("  User Mode Ticks   : %llu\n", cpu.user);
        printf("  System Mode Ticks : %llu\n", cpu.system);
        printf("  Idle Ticks        : %llu\n", cpu.idle);
        printf("  I/O Wait Ticks    : %llu\n", cpu.iowait);
        printf("  Total CPU Ticks   : %llu\n", total_ticks);
        if (total_ticks > 0) {
            printf("  Overall Activity  : %.2f%%\n", ((double)active_ticks / total_ticks) * 100.0);
        }
    } else {
        printf("Failed to retrieve CPU statistics.\n");
    }

    printf("=======================================================\n");
}