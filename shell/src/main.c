/**
 * @file main.c
 * @brief Entry point for the Mini Shell application.
 *
 * Initializes the execution loop of the custom shell interpreter.
 */

#include "../include/shell.h"

/**
 * @brief Main function.
 *
 * Starts the interactive shell Read-Eval-Print Loop (REPL).
 *
 * @return Always returns 0 upon exit.
 */
int main(void) {
    shell_loop();
    return 0;
}