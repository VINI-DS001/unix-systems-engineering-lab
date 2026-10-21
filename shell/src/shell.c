#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../include/shell.h"
#include "../include/builtins.h"
#include "../include/parser.h"

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

int exec_pipeline(pipeline_t *pipe_cmd) {
    int num_cmds = pipe_cmd->num_cmds;
    int pipefds[2 * (num_cmds - 1)];

    /* Create all necessary pipes */
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("minishell: error creating pipe");
            return -1;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("minishell: fork error");
            return -1;
        } else if (pid == 0) {
            /* Child Process i */

            /* If not the first command, connect input to the previous pipe */
            if (i != 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }

            /* If not the last command, connect output to the next pipe */
            if (i != num_cmds - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            /* Close all pipe descriptors in the child */
            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }

            /* Apply file redirections (<, >, >>) if any */
            setup_redirections(&pipe_cmd->cmds[i]);

            if (execvp(pipe_cmd->cmds[i].args[0], pipe_cmd->cmds[i].args) == -1) {
                perror("minishell");
            }
            exit(EXIT_FAILURE);
        }
    }

    /* Parent Process: close all pipes and wait for children */
    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return 0;
}

void shell_loop(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    pipeline_t pipe_cmd;

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

        /* If it is a single command and a builtin */
        if (pipe_cmd.num_cmds == 1 && exec_builtin(pipe_cmd.cmds[0].args)) {
            continue;
        }

        /* Execute command pipeline */
        exec_pipeline(&pipe_cmd);
    }

    free(line);
}