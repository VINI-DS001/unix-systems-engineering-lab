#ifndef SHELL_H
#define SHELL_H

#include "parser.h"

#define MAX_LINE 1024

/* Starts the main Shell loop (Read-Eval-Print Loop) */
void shell_loop(void);

/* Executes an external program with redirection support */
int exec_pipeline(pipeline_t *pipe_cmd);

#endif /* SHELL_H */