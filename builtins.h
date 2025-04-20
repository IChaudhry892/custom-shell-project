#ifndef BUILTINS_H
#define BUILTINS_H

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

// Function Prototypes for 3 Built-In Commands

// cd Command - Ibrahim
void cd_command(char **args);
// exit Command - Achintya
void exit_command();

// path Command - Achintya
void path_command();


// exit Command

// path Command

#endif
