/**
 * @file metrics.h
 * @brief Performance metric calculation interfaces and Gantt chart visualization functions.
 *
 * Provides data structures and functions for computing CPU scheduling statistics,
 * such as average Turnaround Time (TAT), Waiting Time (WT), Response Time (RT),
 * and CPU utilization, as well as rendering ASCII Gantt charts.
 */

#ifndef METRICS_H
#define METRICS_H

#include "process.h"

/**
 * @struct gantt_segment_t
 * @brief Represents a continuous time slice of execution on the CPU.
 */
typedef struct {
    int pid;        /**< PID executed during this slice (-1 indicates IDLE) */
    int start_time; /**< Start time unit of the execution slice */
    int end_time;   /**< End time unit of the execution slice */
} gantt_segment_t;

/**
 * @struct metrics_t
 * @brief Aggregated performance statistics computed over a process workload.
 */
typedef struct {
    double avg_waiting_time;    /**< Average Waiting Time across all processes */
    double avg_turnaround_time; /**< Average Turnaround Time across all processes */
    double avg_response_time;   /**< Average Response Time across all processes */
    double cpu_utilization;     /**< Percentage of time the CPU was actively executing processes */
} metrics_t;

/**
 * @brief Computes individual process statistics and populates overall performance metrics.
 *
 * Calculates Turnaround Time (TAT = completion - arrival), Waiting Time (WT = TAT - burst),
 * and Response Time (RT = start - arrival) for each process, then averages them.
 *
 * @param list Pointer to the process list containing completed execution data.
 * @param out_metrics Pointer to the metrics_t structure to store calculated averages.
 * @param total_simulation_time Total elapsed time units from start to finish of simulation.
 * @param idle_time Total time units the CPU remained idle.
 */
void calculate_metrics(process_list_t *list, metrics_t *out_metrics, int total_simulation_time, int idle_time);

/**
 * @brief Prints a formatted evaluation table and summary of metrics to stdout.
 *
 * @param list Pointer to the process list with final execution state.
 * @param metrics Pointer to the calculated metrics structure.
 * @param algo_name Human-readable string identifier of the scheduling algorithm.
 */
void print_metrics_table(const process_list_t *list, const metrics_t *metrics, const char *algo_name);

/**
 * @brief Renders an ASCII timeline Gantt chart based on execution segments.
 *
 * @param gantt Array of gantt_segment_t tracking CPU execution intervals.
 * @param gantt_size Number of active segments in the gantt array.
 */
void print_gantt_chart(const gantt_segment_t *gantt, int gantt_size);

#endif /* METRICS_H */