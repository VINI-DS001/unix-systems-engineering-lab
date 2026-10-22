/**
 * @file parser.c
 * @brief Command line parsing and tokenization implementation.
 *
 * Parses raw command line input strings into structured pipeline representations,
 * handling pipelines ('|'), I/O redirections ('<', '>', '>>'), and background flags ('&').
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"

/**
 * @brief Helper function to parse a single command string into a command_t structure.
 *
 * Extracts argument strings, input redirection ('<'), output truncation ('>'),
 * and output appending ('>>').
 *
 * @param cmd_str Sub-string representing a single command stage in a pipeline.
 * @param cmd Pointer to command_t structure to store parsed arguments and redirections.
 */
static void parse_single_command(char *cmd_str, command_t *cmd) {
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;

    int argc = 0;
    char *token = strtok(cmd_str, " \t\r\n\a");

    while (token != NULL && argc < 63) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            if (token != NULL) cmd->input_file = token;
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            if (token != NULL) {
                cmd->output_file = token;
                cmd->append = 1;
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            if (token != NULL) {
                cmd->output_file = token;
                cmd->append = 0;
            }
        } else {
            cmd->args[argc++] = token;
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    cmd->args[argc] = NULL;
}

/**
 * @brief Parses an entire command line input into a pipeline_t structure.
 *
 * Checks for background execution indicator ('&'), splits by pipeline operator ('|'),
 * and invokes command parsing for each stage.
 *
 * @param line Raw input string read from stdin.
 * @param pipe_cmd Pointer to target pipeline_t structure.
 */
void parse_pipeline(char *line, pipeline_t *pipe_cmd) {
    pipe_cmd->num_cmds = 0;
    pipe_cmd->background = 0;

    /* Check and remove '&' if present at the end of the line */
    char *bg_ptr = strchr(line, '&');
    if (bg_ptr != NULL) {
        pipe_cmd->background = 1;
        *bg_ptr = '\0'; /* Remove '&' so it doesn't interfere with tokenizer */
    }

    char *cmd_strs[MAX_COMMANDS];
    char *token = strtok(line, "|");

    while (token != NULL && pipe_cmd->num_cmds < MAX_COMMANDS) {
        cmd_strs[pipe_cmd->num_cmds++] = token;
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < pipe_cmd->num_cmds; i++) {
        parse_single_command(cmd_strs[i], &pipe_cmd->cmds[i]);
    }
}