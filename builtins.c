#include "builtins.h"

// Maximum number of paths we can store
#define MAX_PATHS 64

// Array to store the paths
char *path_list[MAX_PATHS];
int path_count = 0;

// Function Implementations for 3 Built-In Commands

// cd Command - Ibrahim
void cd_command(char **args){
    char *directory;

    int argument_count = 0;
    while (args[argument_count] != NULL){
        argument_count++;
    }

    // More than one argument after cd
    if (argument_count > 2){
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    if (args[1] == NULL){   // cd with no argument
        directory = getenv("HOME");
        if (directory == NULL){
            fprintf(stderr, "cd: HOME environment variable not set\n");
            return;
        }
    } else{   // cd with one optional argument
        directory = args[1];
    }

    // Try to change to 'directory'
    if (chdir(directory) != 0){
        perror("cd error");
    }
}

// path Command - Prateek
void path_command() {
    printf("Custom path management not implemented yet.\n");
}

// exit Command - Achintya
void exit_command() {
    printf("Exiting newshell... Goodbye, Achintya!\n");
    exit(EXIT_SUCCESS);
}
