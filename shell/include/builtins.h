/**
 * @file builtins.h
 * @brief Declaration of built-in shell commands and handlers.
 *
 * This header defines the interface for identifying and executing shell
 * built-in commands (cd, pwd, exit) directly within the main process.
 */

#ifndef BUILTINS_H
#define BUILTINS_H

/**
 * @brief Checks if a command is a built-in and executes it.
 *
 * Evaluates the first argument against supported built-in commands
 * (cd, pwd, exit). If a match is found, the command is executed within
 * the current process context.
 *
 * @param args Array of null-terminated string arguments (args[0] is the command).
 * @return 1 if the command was a built-in and was executed; 0 otherwise.
 */
int exec_builtin(char **args);

#endif /* BUILTINS_H */