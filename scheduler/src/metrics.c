/**
 * @file metrics.c
 * @brief Implementation of performance analysis metrics and ASCII Gantt chart rendering.
 *
 * Provides execution time evaluation (Turnaround Time, Waiting Time, Response Time),
 * overall CPU utilization calculation, and terminal visualization of execution timelines.
 */

#include <stdio.h>
#include "../include/metrics.h"

/**
 * @brief Computes individual process timing statistics and overall average metrics.
 *
 * Iterates through all processes in the list to compute:
 * - Turnaround Time (TAT) = completion_time - arrival_time
 * - Waiting Time (WT) = turnaround_time - burst_time
 * - Response Time (RT) = start_time - arrival_time
 * Also calculates global averages and CPU utilization percentage.
 *
 * @param list Pointer to completed process list.
 * @param out_metrics Pointer to metrics_t structure to store computed results.
 * @param total_simulation_time Total time units elapsed during simulation.
 * @param idle_time Total time units the CPU was in IDLE state.
 */
void calculate_metrics(process_list_t *list, metrics_t *out_metrics, int total_simulation_time, int idle_time) {
    if (!list || list->count == 0 || total_simulation_time == 0) return;

    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;

    for (int i = 0; i < list->count; i++) {
        process_t *p = &list->processes[i];
        
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
        total_rt += p->response_time;
    }

    out_metrics->avg_waiting_time = total_wt / list->count;
    out_metrics->avg_turnaround_time = total_tat / list->count;
    out_metrics->avg_response_time = total_rt / list->count;
    out_metrics->cpu_utilization = ((double)(total_simulation_time - idle_time) / total_simulation_time) * 100.0;
}

/**
 * @brief Outputs a detailed evaluation table and summary statistics to standard output.
 *
 * Displays process-level statistics (PID, Arrival, Burst, Priority, Start, End, TAT, WT, RT)
 * alongside aggregated workload metrics.
 *
 * @param list Pointer to process list containing metrics.
 * @param metrics Pointer to summary metrics structure.
 * @param algo_name Name of the scheduling algorithm evaluated.
 */
void print_metrics_table(const process_list_t *list, const metrics_t *metrics, const char *algo_name) {
    printf("\n=======================================================\n");
    printf("         ALGORITHM: %s\n", algo_name);
    printf("=======================================================\n");
    printf("PID | Arrival | Burst | Priority | Start | End | TAT | WT  | RT \n");
    printf("----+---------+-------+----------+-------+-----+-----+-----+----\n");

    for (int i = 0; i < list->count; i++) {
        const process_t *p = &list->processes[i];
        printf("%3d | %7d | %5d | %8d | %5d | %3d | %3d | %3d | %2d\n",
               p->pid, p->arrival_time, p->burst_time, p->priority,
               p->start_time, p->completion_time,
               p->turnaround_time, p->waiting_time, p->response_time);
    }

    printf("-------------------------------------------------------\n");
    printf("Average Waiting Time    (WT)  : %.2f units\n", metrics->avg_waiting_time);
    printf("Average Turnaround Time (TAT) : %.2f units\n", metrics->avg_turnaround_time);
    printf("Average Response Time   (RT)  : %.2f units\n", metrics->avg_response_time);
    printf("CPU Utilization               : %.2f%%\n", metrics->cpu_utilization);
    printf("=======================================================\n");
}

/**
 * @brief Visualizes CPU time slice execution using an ASCII Gantt chart format.
 *
 * Formats timeline blocks showing process executions or IDLE periods with associated
 * start and end timestamp markers.
 *
 * @param gantt Array of execution segments.
 * @param gantt_size Number of active segments in the timeline.
 */
void print_gantt_chart(const gantt_segment_t *gantt, int gantt_size) {
    if (gantt_size == 0) return;

    printf("\n--- Gantt Chart ---\n");

    /* Top border */
    for (int i = 0; i < gantt_size; i++) {
        printf("+-------");
    }
    printf("+\n|");

    /* Process labels */
    for (int i = 0; i < gantt_size; i++) {
        if (gantt[i].pid == -1) {
            printf(" IDLE  |");
        } else {
            printf("  P%-3d |", gantt[i].pid);
        }
    }
    printf("\n");

    /* Bottom border */
    for (int i = 0; i < gantt_size; i++) {
        printf("+-------");
    }
    printf("+\n");

    /* Time markers */
    printf("%-8d", gantt[0].start_time);
    for (int i = 0; i < gantt_size; i++) {
        printf("%-8d", gantt[i].end_time);
    }
    printf("\n\n");
}