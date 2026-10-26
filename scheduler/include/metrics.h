/**
 * @file metrics.h
 * @brief Metric calculation definitions and Gantt chart renderer.
 */

#ifndef METRICS_H
#define METRICS_H

#include "process.h"

/**
 * @struct gantt_segment_t
 * @brief Represents a time slice execution block on the CPU.
 */
typedef struct {
    int pid;        /**< PID executed (-1 for IDLE) */
    int start_time; /**< Start unit of execution */
    int end_time;   /**< End unit of execution */
} gantt_segment_t;

/**
 * @struct metrics_t
 * @brief Contains calculated average performance metrics.
 */
typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double cpu_utilization;
} metrics_t;

void calculate_metrics(process_list_t *list, metrics_t *out_metrics, int total_simulation_time, int idle_time);
void print_metrics_table(const process_list_t *list, const metrics_t *metrics, const char *algo_name);
void print_gantt_chart(const gantt_segment_t *gantt, int gantt_size);

#endif /* METRICS_H */