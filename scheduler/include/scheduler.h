/**
 * @file scheduler.h
 * @brief Declarations of CPU scheduling algorithms (FCFS, SJF, SRTF, RR, Priority).
 *
 * Interfaces for invoking discrete event simulation algorithms on process workloads.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "metrics.h"

/** @brief Maximum capacity for recorded Gantt execution segments. */
#define MAX_GANTT_SEGMENTS 1000

/**
 * @brief Executes First-Come, First-Served (FCFS) scheduling (Non-preemptive).
 *
 * Processes are dispatched strictly by arrival time.
 *
 * @param list Pointer to target process_list_t workload.
 * @param gantt Array buffer to store generated execution timeline segments.
 * @param gantt_size Output pointer receiving total count of generated Gantt segments.
 * @param metrics Output pointer receiving computed workload performance metrics.
 */
void run_fcfs(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

/**
 * @brief Executes Shortest Job First (SJF) scheduling (Non-preemptive).
 *
 * Selects the ready process with the smallest total burst time.
 *
 * @param list Pointer to target process_list_t workload.
 * @param gantt Array buffer to store generated execution timeline segments.
 * @param gantt_size Output pointer receiving total count of generated Gantt segments.
 * @param metrics Output pointer receiving computed workload performance metrics.
 */
void run_sjf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

/**
 * @brief Executes Shortest Remaining Time First (SRTF) scheduling (Preemptive SJF).
 *
 * Always selects the ready process with the shortest remaining execution time,
 * preempting running processes if a shorter process arrives.
 *
 * @param list Pointer to target process_list_t workload.
 * @param gantt Array buffer to store generated execution timeline segments.
 * @param gantt_size Output pointer receiving total count of generated Gantt segments.
 * @param metrics Output pointer receiving computed workload performance metrics.
 */
void run_srtf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

/**
 * @brief Executes Round Robin (RR) scheduling (Preemptive by Time Quantum).
 *
 * Rotates ready processes through a FIFO queue, allocating up to quantum units per turn.
 *
 * @param list Pointer to target process_list_t workload.
 * @param quantum Maximum CPU time slice allocated to each process per cycle.
 * @param gantt Array buffer to store generated execution timeline segments.
 * @param gantt_size Output pointer receiving total count of generated Gantt segments.
 * @param metrics Output pointer receiving computed workload performance metrics.
 */
void run_round_robin(process_list_t *list, int quantum, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

/**
 * @brief Executes Priority Scheduling (Supports Preemptive and Non-preemptive modes).
 *
 * Dispatches processes based on priority integer (lower values indicate higher priority).
 *
 * @param list Pointer to target process_list_t workload.
 * @param preemptive Flag: 1 for preemptive execution, 0 for non-preemptive execution.
 * @param gantt Array buffer to store generated execution timeline segments.
 * @param gantt_size Output pointer receiving total count of generated Gantt segments.
 * @param metrics Output pointer receiving computed workload performance metrics.
 */
void run_priority(process_list_t *list, int preemptive, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics);

#endif /* SCHEDULER_H */