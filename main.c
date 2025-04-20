#include "newshell.h"

int main(int argc, char *argv[]){
    if (argc == 1){
        // Run shell in interactive mode
        printf("CALL INTERACTIVE MODE FUNCTION\n");
        run_interactive_mode();
    }  else if (argc == 2){
        // Run shell in batch mode
        printf("CALL BATCH MODE FUNCTION\n");
        run_batch_mode(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s\tor\t%s [batchfile]\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
