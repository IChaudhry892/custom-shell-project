/*
 * builtins.c - Implements built-in commands for the shell.
 * Ibrahim Chaudhry (cd), Prateek Banda (path), Achintya Yalamati (exit) - CSCE 3600
 */
#include "builtins.h"

// Maximum number of paths we can store
#define MAX_PATHS 64

// Array to store the paths
char *path_list[MAX_PATHS];
int path_count = 0;

char *original_path = NULL;  // Save original PATH to restore later


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
void path_command(char **args) {
    if (args[1] == NULL) {
        // If no arguments, print current path list
        if (path_count == 0) {
            printf("No paths set.\n");
        } else {
            for (int i = 0; i < path_count; i++) {
                printf("%s", path_list[i]);
                if (i < path_count - 1) printf(":");
            }
            printf("\n");
        }
    } else if (strcmp(args[1], "+") == 0 && args[2] != NULL) {
        // Add a new path
        if (path_count < MAX_PATHS) {
            path_list[path_count++] = strdup(args[2]);
            printf("Path added: %s\n", args[2]);
        } else {
            fprintf(stderr, "Error: Maximum number of paths reached.\n");
        }
    } else if (strcmp(args[1], "-") == 0 && args[2] != NULL) {
        // Remove a path
        int found = 0;
        for (int i = 0; i < path_count; i++) {
            if (strcmp(path_list[i], args[2]) == 0) {
                free(path_list[i]);
                // Shift the remaining paths left
                for (int j = i; j < path_count - 1; j++) {
                    path_list[j] = path_list[j + 1];
                }
                path_count--;
                found = 1;
                printf("Path removed: %s\n", args[2]);
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Error: Path not found: %s\n", args[2]);
        }
    } else {
        fprintf(stderr, "Invalid path command usage. Use 'path' to list, 'path + <path>' to add, or 'path - <path>' to remove.\n");
    }
}


// exit Command - Achintya
void exit_command() {
    printf("Exiting newshell... Goodbye!\n");
    exit(EXIT_SUCCESS);
}
