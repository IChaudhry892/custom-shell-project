#ifndef NEWSHELL_H
#define NEWSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

// Command-line input handles up to 512 characters
#define MAX_COMMAND_INPUT_SIZE 512

// Function Prototypes For:

// Running the shell in interactive mode
void run_interactive_mode();

// Running the shell in batch mode
void run_batch_mode();

// Tokenizing a command
char **parse_input(char *command);

// Executing a command
void execute_command(char **args);

// Parsing and executing the input line
void parse_and_execute(char *input_line);

// Executing the 3 built-in commands, maybe we could just do this in parse_and_execute

// Redirection - Ibrahim

// Pipelining

// Signal handling

#endif