/**
 * @file parser.h
 * @brief Command line parsing interface and pipeline data structures.
 *
 * Defines the structures used to represent parsed commands, I/O redirections,
 * and multi-stage execution pipelines, as well as the parser entry point.
 */

#ifndef PARSER_H
#define PARSER_H

/** @brief Maximum number of piped commands supported in a single pipeline. */
#define MAX_COMMANDS 10

/**
 * @struct command_t
 * @brief Represents a single command with its arguments and I/O redirections.
 */
typedef struct {
    char *args[64];      /**< Array of null-terminated argument strings. */
    char *input_file;   /**< Path to input redirection file (<), or NULL. */
    char *output_file;  /**< Path to output redirection file (>/>>), or NULL. */
    int append;          /**< Flag: 1 for append mode (>>), 0 for truncate (>). */
} command_t;

/**
 * @struct pipeline_t
 * @brief Represents a full command execution pipeline with background flag.
 */
typedef struct {
    command_t cmds[MAX_COMMANDS]; /**< Array of command structures in the pipeline. */
    int num_cmds;                  /**< Number of parsed commands in the pipeline. */
    int background;                /**< Flag: 1 if pipeline ends with '&', 0 otherwise. */
} pipeline_t;

/**
 * @brief Tokenizes an input line into a pipeline structure.
 *
 * Parses pipeline separators ('|'), I/O redirections ('<', '>', '>>'),
 * background flags ('&'), and populates the given pipeline_t structure.
 *
 * @param line Modifiable input line string to be parsed.
 * @param pipe_cmd Pointer to the pipeline structure to populate.
 */
void parse_pipeline(char *line, pipeline_t *pipe_cmd);

#endif /* PARSER_H */