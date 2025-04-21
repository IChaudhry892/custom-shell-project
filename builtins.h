/*
 * builtins.h - Header for built-in shell commands.
 * Declares prototypes for cd, exit, and path commands.
 */
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

// path Command - Prateek
void path_command(char **args);  // Updated to take arguments

#endif
