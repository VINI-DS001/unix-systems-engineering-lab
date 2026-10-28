/**
 * @file process.c
 * @brief Process Control Block (PCB) dynamic allocation, memory cleanup, cloning, and state reset helpers.
 *
 * Manages life cycle memory for simulated processes and workload state duplication
 * to allow accurate comparative evaluations across different scheduling algorithms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/**
 * @brief Dynamically allocates a new process list container and process array.
 *
 * @param capacity Initial maximum capacity of processes to allocate.
 * @return Pointer to allocated process_list_t structure, or NULL on allocation failure.
 */
process_list_t *create_process_list(int capacity) {
    process_list_t *list = malloc(sizeof(process_list_t));
    if (!list) return NULL;

    list->processes = malloc(sizeof(process_t) * capacity);
    if (!list->processes) {
        free(list);
        return NULL;
    }
    list->count = 0;
    return list;
}

/**
 * @brief Safely frees memory allocated for a process list and its inner array.
 *
 * @param list Pointer to process_list_t structure to free.
 */
void free_process_list(process_list_t *list) {
    if (list) {
        if (list->processes) {
            free(list->processes);
        }
        free(list);
    }
}

/**
 * @brief Deep-copies a process list and all contained PCB entries.
 *
 * @param source Pointer to source process_list_t structure.
 * @return Pointer to new process_list_t clone, or NULL on error.
 */
process_list_t *clone_process_list(const process_list_t *source) {
    if (!source) return NULL;

    process_list_t *clone = create_process_list(source->count);
    if (!clone) return NULL;

    memcpy(clone->processes, source->processes, sizeof(process_t) * source->count);
    clone->count = source->count;
    return clone;
}

/**
 * @brief Resets simulation state metrics of all processes in the list back to initial values.
 *
 * Restores remaining execution time to initial burst time, sets start time to uninitialized (-1),
 * zeros turnaround/waiting/response times, and sets state to STATE_READY.
 *
 * @param list Pointer to process_list_t to reset.
 */
void reset_process_list(process_list_t *list) {
    if (!list) return;

    for (int i = 0; i < list->count; i++) {
        list->processes[i].remaining_time = list->processes[i].burst_time;
        list->processes[i].start_time = -1;
        list->processes[i].completion_time = 0;
        list->processes[i].waiting_time = 0;
        list->processes[i].turnaround_time = 0;
        list->processes[i].response_time = 0;
        list->processes[i].state = STATE_READY;
    }
}