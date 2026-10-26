/**
 * @file process.h
 * @brief Definition of Process Control Block (PCB) and process list structures.
 */

#ifndef PROCESS_H
#define PROCESS_H

/**
 * @enum process_state_t
 * @brief States of a process during execution simulation.
 */
typedef enum {
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
} process_state_t;

/**
 * @struct process_t
 * @brief Structure representing a Simulated Process (PCB).
 */
typedef struct {
    int pid;                  /**< Process Identifier */
    int arrival_time;         /**< Time when process arrives in ready queue */
    int burst_time;           /**< Total CPU time required */
    int remaining_time;       /**< Remaining CPU time to complete execution */
    int priority;             /**< Priority level (lower integer = higher priority) */
    
    int start_time;           /**< Time when process first executed on CPU (-1 if not started) */
    int completion_time;      /**< Time when process finished execution */
    int waiting_time;         /**< Total time spent waiting in ready queue */
    int turnaround_time;      /**< Total time from arrival to completion */
    int response_time;        /**< Time from arrival to first execution */

    process_state_t state;    /**< Current state of the process */
} process_t;

/**
 * @struct process_list_t
 * @brief Array structure to manage a collection of processes.
 */
typedef struct {
    process_t *processes;
    int count;
} process_list_t;

/* Helper functions for process management */
process_list_t *create_process_list(int capacity);
void free_process_list(process_list_t *list);
process_list_t *clone_process_list(const process_list_t *source);
void reset_process_list(process_list_t *list);

#endif /* PROCESS_H */