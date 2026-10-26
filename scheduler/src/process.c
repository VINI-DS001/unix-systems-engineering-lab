/**
 * @file process.c
 * @brief Memory allocation and state reset helpers for simulated processes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

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

void free_process_list(process_list_t *list) {
    if (list) {
        if (list->processes) {
            free(list->processes);
        }
        free(list);
    }
}

process_list_t *clone_process_list(const process_list_t *source) {
    if (!source) return NULL;

    process_list_t *clone = create_process_list(source->count);
    if (!clone) return NULL;

    memcpy(clone->processes, source->processes, sizeof(process_t) * source->count);
    clone->count = source->count;
    return clone;
}

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