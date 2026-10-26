/**
 * @file scheduler.h
 * @brief CPU Scheduling algorithms declaration and Gantt generation interfaces.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "metrics.h"

#define MAX_GANTT_SEGMENTS 1000

/* Algoritmos de Escalonamento */
void run_fcfs(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);
void run_sjf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);
void run_srtf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);
void run_round_robin(process_list_t *list, int quantum, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);
void run_priority(process_list_t *list, int preemptive, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

#endif /* SCHEDULER_H */