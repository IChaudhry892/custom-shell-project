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

// Command-line input handles up 512 characters
#define MAX_COMMAND_INPUT_SIZE = 512

// Function Prototypes For:

// Running the shell in interactive mode

// Running the shell in batch mode

// Executing commands

// Executing the 3 built-in commands

// Redirection

// Pipelining

// Signal handling

#endif