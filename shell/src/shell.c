#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/shell.h"
#include "../include/builtins.h"

static void parse_line(char *line, char **args) {
    int i = 0;
    char *token = strtok(line, " \t\r\n\a");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n\a");
    }
    args[i] = NULL;
}

void shell_loop(void) {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    char *args[MAX_ARGS];

    while (1) {
        // Display interactive prompt only if input is a TTY terminal
        if (isatty(STDIN_FILENO)) {
            printf("minishell> ");
            fflush(stdout);
        }

        characters = getline(&line, &bufsize, stdin);

        // Handle EOF (Ctrl+D) or read error
        if (characters == -1) {
            if (isatty(STDIN_FILENO)) {
                printf("\n");
            }
            break;
        }

        parse_line(line, args);

        if (args[0] == NULL) {
            continue; // Empty line
        }

        // Try to execute as a builtin command
        if (!exec_builtin(args)) {
            printf("minishell: command not found or not implemented: %s\n", args[0]);
        }
    }

    free(line);
}