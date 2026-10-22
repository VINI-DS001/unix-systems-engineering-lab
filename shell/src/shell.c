/**
 * @file shell.c
 * @brief Core implementation of the Unix Mini Shell execution loop and process pipeline.
 *
 * Implements the Read-Eval-Print Loop (REPL), signal handlers for SIGCHLD and SIGINT,
 * I/O redirection setup, and pipeline process orchestration via fork(), execvp(), 
 * pipe(), and wait().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../include/shell.h"
#include "../include/builtins.h"
#include "../include/parser.h"

/**
 * @brief Signal handler for SIGCHLD to reap terminated child processes.
 *
 * Uses waitpid() with WNOHANG in a non-blocking loop to clean up background
 * processes immediately upon termination, preventing zombie processes.
 *
 * @param sig Signal number received (SIGCHLD).
 */
static void handle_sigchld(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C) when the shell is idle at the prompt.
 *
 * Safely outputs a new line and re-renders the prompt using async-signal-safe
 * write() call, preventing shell termination on interrupt.
 *
 * @param sig Signal number received (SIGINT).
 */
static void handle_sigint(int sig) {
    (void)sig;
    if (isatty(STDIN_FILENO)) {
        write(STDOUT_FILENO, "\nminishell> ", 12);
    }
}

/**
 * @brief Configures input and output file redirections for a single command.
 *
 * Opens specified input/output files and replaces STDIN_FILENO or STDOUT_FILENO
 * using dup2(). Executed inside child processes prior to calling execvp().
 *
 * @param cmd Pointer to command structure containing redirection specs.
 */
static void setup_redirections(command_t *cmd) {
    if (cmd->input_file != NULL) {
        int fd_in = open(cmd->input_file, O_RDONLY);
        if (fd_in < 0) {
            perror("minishell: error opening input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (cmd->output_file != NULL) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (cmd->append) ? O_APPEND : O_TRUNC;
        int fd_out = open(cmd->output_file, flags, 0644);
        if (fd_out < 0) {
            perror("minishell: error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}

/**
 * @brief Executes a pipeline of commands with support for IPC pipes, I/O redirections, and background execution.
 *
 * Creates inter-process communication pipes via pipe(), forks child processes,
 * routes stdin/stdout descriptors, and enforces signal handling policies
 * (ignoring SIGINT in parent during foreground execution).
 *
 * @param pipe_cmd Pointer to the parsed pipeline configuration structure.
 * @return 0 on success, or -1 on system call failure.
 */
int exec_pipeline(pipeline_t *pipe_cmd) {
    int num_cmds = pipe_cmd->num_cmds;
    int pipefds[2 * (num_cmds - 1)];
    pid_t last_pid = 0;

    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("minishell: error creating pipe");
            return -1;
        }
    }

    /* Ignore SIGINT in the parent shell during child execution */
    struct sigaction sa_ignore, sa_old;
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    sigaction(SIGINT, &sa_ignore, &sa_old);

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("minishell: fork error");
            /* Restore previous SIGINT handler in case of error */
            sigaction(SIGINT, &sa_old, NULL);
            return -1;
        } else if (pid == 0) {
            /* In the child process, restore default SIGINT action (SIG_DFL) */
            signal(SIGINT, SIG_DFL);

            if (i != 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i != num_cmds - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }

            setup_redirections(&pipe_cmd->cmds[i]);

            if (execvp(pipe_cmd->cmds[i].args[0], pipe_cmd->cmds[i].args) == -1) {
                perror("minishell");
            }
            exit(EXIT_FAILURE);
        }
        last_pid = pid;
    }

    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    /* If the command is NOT in the background, wait for processes to finish */
    if (!pipe_cmd->background) {
        for (int i = 0; i < num_cmds; i++) {
            int status;
            wait(&status);

            if (WIFSIGNALED(status) && isatty(STDIN_FILENO)) {
                write(STDOUT_FILENO, "\n", 1);
            }
        }
    } else {
        printf("[Background process started with PID %d]\n", last_pid);
    }

    /* Restore original parent shell SIGINT handler */
    sigaction(SIGINT, &sa_old, NULL);

    return 0;
}

/**
 * @brief Starts and manages the main Read-Eval-Print Loop (REPL).
 *
 * Configures signal actions (SIGCHLD and SIGINT), displays prompt dynamically when
 * connected to a TTY, reads user input via getline(), invokes tokenization, and routes
 * execution to built-in handlers or process pipelines.
 */
void shell_loop(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    pipeline_t pipe_cmd;

    /* Configure POSIX signal handlers */
    struct sigaction sa_chld, sa_int;

    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    while (1) {
        if (isatty(STDIN_FILENO)) {
            printf("minishell> ");
            fflush(stdout);
        }

        characters = getline(&line, &bufsize, stdin);

        if (characters == -1) {
            if (isatty(STDIN_FILENO)) {
                printf("\n");
            }
            break;
        }

        parse_pipeline(line, &pipe_cmd);

        if (pipe_cmd.num_cmds == 0 || pipe_cmd.cmds[0].args[0] == NULL) {
            continue;
        }

        if (pipe_cmd.num_cmds == 1 && exec_builtin(pipe_cmd.cmds[0].args)) {
            continue;
        }

        exec_pipeline(&pipe_cmd);
    }

    free(line);
}