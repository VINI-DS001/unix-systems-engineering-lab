/**
 * @file sys_info.h
 * @brief Interfaces for extracting system and kernel metrics from /proc virtual filesystem.
 */

#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <stddef.h>

/**
 * @struct sys_mem_info_t
 * @brief Represents RAM utilization metrics extracted from /proc/meminfo.
 */
typedef struct {
    unsigned long total_ram_kb;  /**< Total usable RAM in kilobytes */
    unsigned long free_ram_kb;   /**< Free physical RAM in kilobytes */
    unsigned long avail_ram_kb;  /**< Memory available for starting new apps */
    unsigned long buffers_kb;    /**< Memory used by kernel buffers */
    unsigned long cached_kb;     /**< Memory used by page cache */
} sys_mem_info_t;

/**
 * @struct sys_cpu_info_t
 * @brief Represents total CPU time counters extracted from /proc/stat.
 */
typedef struct {
    unsigned long long user;    /**< Time spent in user mode */
    unsigned long long nice;    /**< Time spent in user mode with low priority */
    unsigned long long system;  /**< Time spent in system (kernel) mode */
    unsigned long long idle;    /**< Time spent in the idle task */
    unsigned long long iowait;  /**< Time spent waiting for I/O to complete */
} sys_cpu_info_t;

/**
 * @brief Reads /proc/meminfo and populates the memory metrics structure.
 *
 * @param info Pointer to sys_mem_info_t structure to store parsed metrics.
 * @return 0 on success, or -1 on file read/parsing error.
 */
int get_system_memory_info(sys_mem_info_t *info);

/**
 * @brief Reads /proc/stat and captures current CPU tick counters.
 *
 * @param info Pointer to sys_cpu_info_t structure to store parsed metrics.
 * @return 0 on success, or -1 on file read/parsing error.
 */
int get_system_cpu_info(sys_cpu_info_t *info);

/**
 * @brief Prints a formatted diagnostic dashboard of current system metrics.
 */
void print_system_diagnostics(void);

#endif /* SYS_INFO_H */