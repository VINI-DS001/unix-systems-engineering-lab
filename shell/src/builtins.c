#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/builtins.h"

int exec_builtin(char **args) {
    if (args[0] == NULL) {
        return 0;
    }

    /* Builtin command: exit */
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    /* Builtin command: pwd */
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd error");
        }
        return 1;
    }

    /* Builtin command: cd */
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            char *home = getenv("HOME");
            if (home != NULL) {
                if (chdir(home) != 0) {
                    perror("cd error");
                }
            }
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd error");
            }
        }
        return 1;
    }

    return 0; /* Not a builtin */
}