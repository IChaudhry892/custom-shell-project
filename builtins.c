#include "builtins.h"

// Maximum number of paths we can store
#define MAX_PATHS 64

// Array to store the paths
char *path_list[MAX_PATHS];
int path_count = 0;

// Function Implementations for 3 Built-In Commands

// cd Command - Ibrahim
void cd_command(){
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
