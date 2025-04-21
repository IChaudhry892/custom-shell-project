/*
 * main.c - Entry point for the custom shell program.
 * Ibrahim Chaudhry - CSCE 3600
 * Calls interactive or batch mode based on command-line arguments.
 */
#include "newshell.h"

int main(int argc, char *argv[]){
    if (argc == 1){
        // Run shell in interactive mode
        run_interactive_mode();
    } else if (argc == 2){
        // Run shell in batch mode
        run_batch_mode(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s\tor\t%s [batchfile]\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
