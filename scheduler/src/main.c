/**
 * @file main.c
 * @brief Entry point for the CPU Scheduler Simulator.
 *
 * Demonstrates FCFS, SJF, SRTF, Round Robin, and Priority scheduling algorithms
 * on a benchmark set of simulated processes. Supports loading workloads from text files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"
#include "../include/metrics.h"
#include "../include/scheduler.h"

/**
 * @brief Loads default sample processes into the process list.
 *
 * Initializes a standard 5-process test workload with varying arrival times,
 * burst times, and priorities to serve as a baseline benchmark.
 *
 * @param list Pointer to pre-allocated process_list_t structure.
 */
static void load_sample_processes(process_list_t *list) {
    list->count = 5;
    
    /* P1: Arrival = 0, Burst = 8, Priority = 3 */
    list->processes[0] = (process_t){ .pid = 1, .arrival_time = 0, .burst_time = 8, .priority = 3 };
    /* P2: Arrival = 1, Burst = 4, Priority = 1 */
    list->processes[1] = (process_t){ .pid = 2, .arrival_time = 1, .burst_time = 4, .priority = 1 };
    /* P3: Arrival = 2, Burst = 9, Priority = 4 */
    list->processes[2] = (process_t){ .pid = 3, .arrival_time = 2, .burst_time = 9, .priority = 4 };
    /* P4: Arrival = 3, Burst = 5, Priority = 2 */
    list->processes[3] = (process_t){ .pid = 4, .arrival_time = 3, .burst_time = 5, .priority = 2 };
    /* P5: Arrival = 4, Burst = 2, Priority = 5 */
    list->processes[4] = (process_t){ .pid = 5, .arrival_time = 4, .burst_time = 2, .priority = 5 };

    reset_process_list(list);
}

/**
 * @brief Parses and loads process definitions from a plain text file.
 *
 * Reads formatted records line-by-line where each line specifies:
 * `PID Arrival_Time Burst_Time Priority`.
 *
 * @param filename Path to the input dataset file.
 * @param list Pointer to target process_list_t structure.
 * @return 0 on success, or -1 if file opening fails.
 */
static int load_processes_from_file(const char *filename, process_list_t *list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening input file");
        return -1;
    }

    int pid, arr, burst, prio;
    int count = 0;

    while (fscanf(fp, "%d %d %d %d", &pid, &arr, &burst, &prio) == 4 && count < 100) {
        list->processes[count] = (process_t){
            .pid = pid,
            .arrival_time = arr,
            .burst_time = burst,
            .priority = prio
        };
        count++;
    }

    fclose(fp);
    list->count = count;
    reset_process_list(list);
    return 0;
}

/**
 * @brief Main execution function.
 *
 * Allocates process lists, loads process dataset, and sequentially invokes each
 * scheduling algorithm (FCFS, SJF, SRTF, Round Robin, Priority Non-Preemptive, Priority Preemptive),
 * outputting performance metrics and Gantt charts for each run.
 *
 * @param argc Command line argument count.
 * @param argv Command line argument vector (optional argv[1] as input filename).
 * @return EXIT_SUCCESS on successful completion, or EXIT_FAILURE on error.
 */
int main(int argc, char *argv[]) {
    printf("=======================================================\n");
    printf("        CPU SCHEDULER SIMULATOR (C17 / POSIX)         \n");
    printf("=======================================================\n");

    process_list_t *master_list = create_process_list(100);
    if (!master_list) {
        fprintf(stderr, "Memory allocation error.\n");
        return EXIT_FAILURE;
    }

    if (argc > 1) {
        printf("Loading processes from file: %s\n", argv[1]);
        if (load_processes_from_file(argv[1], master_list) != 0) {
            free_process_list(master_list);
            return EXIT_FAILURE;
        }
    } else {
        printf("No input file provided. Using default sample process workload.\n");
        load_sample_processes(master_list);
    }

    gantt_segment_t gantt[MAX_GANTT_SEGMENTS];
    int gantt_size = 0;
    metrics_t metrics;

    /* 1. First-Come, First-Served (FCFS) */
    process_list_t *work_list = clone_process_list(master_list);
    run_fcfs(work_list, gantt, &gantt_size, &metrics);
    print_metrics_table(work_list, &metrics, "First-Come, First-Served (FCFS)");
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    /* 2. Shortest Job First (SJF) */
    work_list = clone_process_list(master_list);
    run_sjf(work_list, gantt, &gantt_size, &metrics);
    print_metrics_table(work_list, &metrics, "Shortest Job First (SJF) - Non-preemptive");
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    /* 3. Shortest Remaining Time First (SRTF) */
    work_list = clone_process_list(master_list);
    run_srtf(work_list, gantt, &gantt_size, &metrics);
    print_metrics_table(work_list, &metrics, "Shortest Remaining Time First (SRTF) - Preemptive");
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    /* 4. Round Robin (RR) - Quantum = 2 */
    int quantum = 2;
    work_list = clone_process_list(master_list);
    run_round_robin(work_list, quantum, gantt, &gantt_size, &metrics);
    char rr_title[64];
    snprintf(rr_title, sizeof(rr_title), "Round Robin (RR) [Quantum = %d]", quantum);
    print_metrics_table(work_list, &metrics, rr_title);
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    /* 5. Priority Scheduling (Non-Preemptive) */
    work_list = clone_process_list(master_list);
    run_priority(work_list, 0, gantt, &gantt_size, &metrics);
    print_metrics_table(work_list, &metrics, "Priority Scheduling (Non-Preemptive)");
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    /* 6. Priority Scheduling (Preemptive) */
    work_list = clone_process_list(master_list);
    run_priority(work_list, 1, gantt, &gantt_size, &metrics);
    print_metrics_table(work_list, &metrics, "Priority Scheduling (Preemptive)");
    print_gantt_chart(gantt, gantt_size);
    free_process_list(work_list);

    free_process_list(master_list);
    return EXIT_SUCCESS;
}