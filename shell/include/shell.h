/**
 * @file shell.h
 * @brief Core shell execution loop and process pipeline interface.
 *
 * Defines main loop functions and process management functions for the
 * Mini Shell implementation.
 */

#ifndef SHELL_H
#define SHELL_H

#include "parser.h"

/** @brief Maximum buffer size for input command lines. */
#define MAX_LINE 1024

/**
 * @brief Starts the Read-Eval-Print Loop (REPL) of the shell.
 *
 * Initializes POSIX signal handlers, reads command lines interactively,
 * invokes the parser, and dispatches built-in or external execution.
 */
void shell_loop(void);

/**
 * @brief Executes a pipeline of commands using POSIX process management.
 *
 * Creates child processes via fork(), establishes pipes via pipe(),
 * configures file redirections via dup2(), and manages foreground/background
 * execution with waitpid().
 *
 * @param pipe_cmd Pointer to the parsed pipeline structure to execute.
 * @return 0 on success, or -1 on error.
 */
int exec_pipeline(pipeline_t *pipe_cmd);

#endif /* SHELL_H */