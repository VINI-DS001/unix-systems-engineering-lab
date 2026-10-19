#ifndef BUILTINS_H
#define BUILTINS_H

/* Returns 1 if the command is a builtin and was executed, or 0 otherwise */
int exec_builtin(char **args);

#endif /* BUILTINS_H */