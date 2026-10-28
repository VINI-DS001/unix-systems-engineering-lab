/**
 * @file process.h
 * @brief Process Control Block (PCB) structure definition and process list helpers.
 *
 * Defines the simulated process representation, execution states, and helper interfaces
 * for memory management, cloning, and state reset across simulation iterations.
 */

#ifndef PROCESS_H
#define PROCESS_H

/**
 * @enum process_state_t
 * @brief Life cycle states of a simulated process.
 */
typedef enum {
    STATE_READY,     /**< Process arrived and waiting in ready queue */
    STATE_RUNNING,   /**< Process currently executing on the CPU */
    STATE_WAITING,   /**< Process blocked or waiting for I/O (reserved) */
    STATE_TERMINATED /**< Process completed execution */
} process_state_t;

/**
 * @struct process_t
 * @brief Process Control Block (PCB) maintaining simulation attributes and performance metrics.
 */
typedef struct {
    int pid;             /**< Process Identifier */
    int arrival_time;    /**< Time unit when process enters the ready queue */
    int burst_time;      /**< Total CPU execution time required */
    int remaining_time;  /**< Remaining CPU time to finish execution */
    int priority;        /**< Priority integer (lower numerical value = higher priority) */
    
    int start_time;      /**< First time unit when process gained CPU (-1 if unstarted) */
    int completion_time; /**< Time unit when process finished execution */
    int waiting_time;    /**< Total duration spent waiting in ready queue */
    int turnaround_time; /**< Total duration from arrival to completion */
    int response_time;   /**< Duration from arrival until first execution */

    process_state_t state; /**< Current lifecycle state */
} process_t;

/**
 * @struct process_list_t
 * @brief Dynamic array container for managing a collection of processes.
 */
typedef struct {
    process_t *processes; /**< Pointer to allocated array of processes */
    int count;            /**< Current count of processes in list */
} process_list_t;

/**
 * @brief Allocates and initializes a new process list.
 *
 * @param capacity Maximum number of process_t elements to pre-allocate.
 * @return Pointer to newly allocated process_list_t, or NULL on memory error.
 */
process_list_t *create_process_list(int capacity);

/**
 * @brief Frees all memory allocated for a process list and its inner array.
 *
 * @param list Pointer to process_list_t to deallocate.
 */
void free_process_list(process_list_t *list);

/**
 * @brief Creates a deep copy of a process list.
 *
 * Useful for executing different scheduling algorithms on identical initial workloads.
 *
 * @param source Pointer to source process_list_t to duplicate.
 * @return Pointer to newly allocated clone process_list_t, or NULL on error.
 */
process_list_t *clone_process_list(const process_list_t *source);

/**
 * @brief Resets calculated metric fields of all processes back to initial state.
 *
 * Re-initializes remaining_time to burst_time, sets start_time to -1, clears times,
 * and restores state to STATE_READY.
 *
 * @param list Pointer to process_list_t to reset.
 */
void reset_process_list(process_list_t *list);

#endif /* PROCESS_H */