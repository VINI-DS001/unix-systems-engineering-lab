/**
 * @file algorithms.c
 * @brief Implementation of CPU scheduling algorithms (FCFS, SJF, SRTF, RR, Priority).
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/scheduler.h"

/**
 * @brief Adds an execution segment to the Gantt chart, merging continuous blocks.
 */
static void add_gantt_segment(gantt_segment_t *gantt, int *gantt_size, int pid, int start_time, int end_time) {
    if (start_time == end_time) return;

    /* Merge with the previous segment if it is the same process */
    if (*gantt_size > 0 && gantt[*gantt_size - 1].pid == pid) {
        gantt[*gantt_size - 1].end_time = end_time;
    } else if (*gantt_size < MAX_GANTT_SEGMENTS) {
        gantt[*gantt_size].pid = pid;
        gantt[*gantt_size].start_time = start_time;
        gantt[*gantt_size].end_time = end_time;
        (*gantt_size)++;
    }
}

/* -------------------------------------------------------------------------
 * 1. FCFS (First-Come, First-Served) - Non-preemptive
 * ------------------------------------------------------------------------- */
void run_fcfs(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics) {
    if (!list || list->count == 0) return;

    reset_process_list(list);
    *gantt_size = 0;

    /* Sort processes by arrival time (simple Bubble Sort) */
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = 0; j < list->count - i - 1; j++) {
            if (list->processes[j].arrival_time > list->processes[j + 1].arrival_time) {
                process_t temp = list->processes[j];
                list->processes[j] = list->processes[j + 1];
                list->processes[j + 1] = temp;
            }
        }
    }

    int current_time = 0;
    int idle_time = 0;
    int completed = 0;

    while (completed < list->count) {
        int idx = -1;

        /* Find the next process that has arrived */
        for (int i = 0; i < list->count; i++) {
            if (list->processes[i].state != STATE_TERMINATED && list->processes[i].arrival_time <= current_time) {
                idx = i;
                break;
            }
        }

        /* Idle CPU */
        if (idx == -1) {
            add_gantt_segment(gantt, gantt_size, -1, current_time, current_time + 1);
            current_time++;
            idle_time++;
            continue;
        }

        process_t *p = &list->processes[idx];
        p->start_time = current_time;
        p->completion_time = current_time + p->burst_time;
        p->state = STATE_TERMINATED;

        add_gantt_segment(gantt, gantt_size, p->pid, p->start_time, p->completion_time);

        current_time = p->completion_time;
        completed++;
    }

    calculate_metrics(list, metrics, current_time, idle_time);
}

/* -------------------------------------------------------------------------
 * 2. SJF (Shortest Job First) - Non-preemptive
 * ------------------------------------------------------------------------- */
void run_sjf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics) {
    if (!list || list->count == 0) return;

    reset_process_list(list);
    *gantt_size = 0;

    int current_time = 0;
    int idle_time = 0;
    int completed = 0;

    while (completed < list->count) {
        int shortest_idx = -1;
        int min_burst = INT_MAX;

        /* Select the ready process with the shortest burst time */
        for (int i = 0; i < list->count; i++) {
            if (list->processes[i].state != STATE_TERMINATED && list->processes[i].arrival_time <= current_time) {
                if (list->processes[i].burst_time < min_burst) {
                    min_burst = list->processes[i].burst_time;
                    shortest_idx = i;
                }
            }
        }

        /* Idle CPU */
        if (shortest_idx == -1) {
            add_gantt_segment(gantt, gantt_size, -1, current_time, current_time + 1);
            current_time++;
            idle_time++;
            continue;
        }

        process_t *p = &list->processes[shortest_idx];
        p->start_time = current_time;
        p->completion_time = current_time + p->burst_time;
        p->state = STATE_TERMINATED;

        add_gantt_segment(gantt, gantt_size, p->pid, p->start_time, p->completion_time);

        current_time = p->completion_time;
        completed++;
    }

    calculate_metrics(list, metrics, current_time, idle_time);
}

/* -------------------------------------------------------------------------
 * 3. SRTF (Shortest Remaining Time First) - Preemptive
 * ------------------------------------------------------------------------- */
void run_srtf(process_list_t *list, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics) {
    if (!list || list->count == 0) return;

    reset_process_list(list);
    *gantt_size = 0;

    int current_time = 0;
    int idle_time = 0;
    int completed = 0;

    while (completed < list->count) {
        int shortest_idx = -1;
        int min_remaining = INT_MAX;

        /* Find the ready process with the lowest remaining_time */
        for (int i = 0; i < list->count; i++) {
            if (list->processes[i].remaining_time > 0 && list->processes[i].arrival_time <= current_time) {
                if (list->processes[i].remaining_time < min_remaining) {
                    min_remaining = list->processes[i].remaining_time;
                    shortest_idx = i;
                }
            }
        }

        /* Idle CPU */
        if (shortest_idx == -1) {
            add_gantt_segment(gantt, gantt_size, -1, current_time, current_time + 1);
            current_time++;
            idle_time++;
            continue;
        }

        process_t *p = &list->processes[shortest_idx];

        /* Record the first execution time if it hasn't started yet */
        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        /* Execute for 1 time unit */
        p->remaining_time--;
        add_gantt_segment(gantt, gantt_size, p->pid, current_time, current_time + 1);
        current_time++;

        /* Process finished */
        if (p->remaining_time == 0) {
            p->completion_time = current_time;
            p->state = STATE_TERMINATED;
            completed++;
        }
    }

    calculate_metrics(list, metrics, current_time, idle_time);
}

/* -------------------------------------------------------------------------
 * 4. Round Robin (RR) - Preemptive by Quantum
 * ------------------------------------------------------------------------- */
void run_round_robin(process_list_t *list, int quantum, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics) {
    if (!list || list->count == 0 || quantum <= 0) return;

    reset_process_list(list);
    *gantt_size = 0;

    int current_time = 0;
    int idle_time = 0;
    int completed = 0;

    /* Simple Circular Queue */
    int queue[1000];
    int head = 0, tail = 0;
    int in_queue[1000] = {0};

    /* Add processes that arrive at time 0 */
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].arrival_time <= current_time) {
            queue[tail++] = i;
            in_queue[i] = 1;
        }
    }

    while (completed < list->count) {
        if (head == tail) {
            /* Empty queue -> Idle CPU */
            add_gantt_segment(gantt, gantt_size, -1, current_time, current_time + 1);
            current_time++;
            idle_time++;

            /* Check if new processes have arrived */
            for (int i = 0; i < list->count; i++) {
                if (list->processes[i].remaining_time > 0 && !in_queue[i] && list->processes[i].arrival_time <= current_time) {
                    queue[tail++] = i;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[head++];
        process_t *p = &list->processes[idx];

        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        int exec_time = (p->remaining_time > quantum) ? quantum : p->remaining_time;
        p->remaining_time -= exec_time;

        add_gantt_segment(gantt, gantt_size, p->pid, current_time, current_time + exec_time);
        current_time += exec_time;

        /* Add new processes that arrived DURING the quantum execution */
        for (int i = 0; i < list->count; i++) {
            if (i != idx && list->processes[i].remaining_time > 0 && !in_queue[i] && list->processes[i].arrival_time <= current_time) {
                queue[tail++] = i;
                in_queue[i] = 1;
            }
        }

        /* If not finished yet, return to the end of the queue */
        if (p->remaining_time > 0) {
            queue[tail++] = idx;
        } else {
            p->completion_time = current_time;
            p->state = STATE_TERMINATED;
            completed++;
        }
    }

    calculate_metrics(list, metrics, current_time, idle_time);
}

/* -------------------------------------------------------------------------
 * 5. Priority Scheduling (Preemptive and Non-Preemptive)
 * Note: Lower numerical value indicates higher priority (e.g., 1 > 5).
 * ------------------------------------------------------------------------- */
void run_priority(process_list_t *list, int preemptive, gantt_segment_t *gantt, int *gantt_size, metrics_t *metrics) {
    if (!list || list->count == 0) return;

    reset_process_list(list);
    *gantt_size = 0;

    int current_time = 0;
    int idle_time = 0;
    int completed = 0;

    while (completed < list->count) {
        int highest_priority_idx = -1;
        int min_priority = INT_MAX;

        /* Select the ready process with the highest priority (lowest number) */
        for (int i = 0; i < list->count; i++) {
            if (list->processes[i].remaining_time > 0 && list->processes[i].arrival_time <= current_time) {
                if (list->processes[i].priority < min_priority) {
                    min_priority = list->processes[i].priority;
                    highest_priority_idx = i;
                }
            }
        }

        /* Idle CPU */
        if (highest_priority_idx == -1) {
            add_gantt_segment(gantt, gantt_size, -1, current_time, current_time + 1);
            current_time++;
            idle_time++;
            continue;
        }

        process_t *p = &list->processes[highest_priority_idx];

        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        if (preemptive) {
            /* Preemptive Mode: runs 1 time unit */
            p->remaining_time--;
            add_gantt_segment(gantt, gantt_size, p->pid, current_time, current_time + 1);
            current_time++;

            if (p->remaining_time == 0) {
                p->completion_time = current_time;
                p->state = STATE_TERMINATED;
                completed++;
            }
        } else {
            /* Non-Preemptive Mode: runs to completion */
            int exec_time = p->remaining_time;
            p->remaining_time = 0;
            p->completion_time = current_time + exec_time;
            p->state = STATE_TERMINATED;

            add_gantt_segment(gantt, gantt_size, p->pid, current_time, p->completion_time);
            current_time = p->completion_time;
            completed++;
        }
    }

    calculate_metrics(list, metrics, current_time, idle_time);
}