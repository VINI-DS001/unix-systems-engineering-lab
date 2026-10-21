#ifndef PARSER_H
#define PARSER_H

#define MAX_COMMANDS 10

/* Structure to represent a single command with its arguments and redirections */
typedef struct {
    char *args[64];
    char *input_file;
    char *output_file;
    int append;
} command_t;

/* Structure to represent a command pipeline (e.g., cmd1 | cmd2 | cmd3) */
typedef struct {
    command_t cmds[MAX_COMMANDS];
    int num_cmds;
    int background; /* 1 if the command ends with '&', 0 otherwise */
} pipeline_t;

/* Parses the line and populates the pipeline structure */
void parse_pipeline(char *line, pipeline_t *pipe_cmd);

#endif /* PARSER_H */