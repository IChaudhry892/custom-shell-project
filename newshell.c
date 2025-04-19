#include "newshell.h"

// Function Implementations For:

// Running the shell in interactive mode
void run_interactive_mode(){
    char input_line[MAX_COMMAND_INPUT_SIZE];    // User input
    while (1){
        // Prompt user for input
        printf("newshell>> ");

        if (fgets(input_line, MAX_COMMAND_INPUT_SIZE, stdin) == NULL){
            // handle error or end of file
            break;
        }
        input_line[strcspn(input_line, "\n")] = '\0';   // Remove the newline character at the end of fgets() input

        if (input_line == 0){   // If user input was empty, re-prompt
            continue;
        }

        // *** FOR DEBUGGING ***
        // printf("%s\n", input_line);

        /*
        TODO: Parse user input and execute commands
        parse_and_execute(input_line);
        */
    }
}

// Running the shell in batch mode
void run_batch_mode(){

}

// Executing commands
void parse_and_execute(){
    
}

// Executing the 3 built-in commands

// Redirection

// Pipelining

// Signal handling