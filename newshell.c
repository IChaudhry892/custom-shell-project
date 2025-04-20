#include "newshell.h"

// Function Implementations For:

// Running the shell in interactive mode
void run_interactive_mode(){
    char input_line[MAX_COMMAND_INPUT_SIZE];    // Character array to store user input
    while (1){
        // Prompt user for input
        printf("newshell>> ");
        if (fgets(input_line, MAX_COMMAND_INPUT_SIZE, stdin) == NULL){  // Gets user input
            // handle error or end of file
            break;
        }
        input_line[strcspn(input_line, "\n")] = '\0';   // Remove the newline character at the end of fgets() input

        if (input_line == 0){   // Ignore empty user input, re-prompt
            continue;
        }

        // *** FOR DEBUGGING ***
        // printf("%s\n", input_line);
        // char **args = parse_input(input_line);
        // *** FOR DEBUGGING ***

        /*
        TODO: Parse user input and execute commands
        parse_and_execute(input_line);
        */
    }
}

// Running the shell in batch mode
void run_batch_mode(){

}

// Tokenizing a command
char **parse_input(char *command){
    char **args = malloc(64 * sizeof(char *)); // Allocates memory for an array of 64 string pointers to hold parse arguments (e.g., "ls", "-l", NULL)
    if (args == NULL){  // If memory allocation failed
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int index = 0;  // Keeps track of the number of arguments passed so far
    char *token = strtok(command, " \t");   // Splits the command string at spaces or tabs

    while (token != NULL){
        // printf("Token %d: %s\n", index+1, token); // *** FOR DEBUGGING ***
        args[index] = token;    // Add current token to the args array
        token = strtok(NULL, " \t");    // Move to the next token
        index++;
    }

    args[index] = NULL; // Last argument should be NULL for args to work with execvp()
    // *** FOR DEBUGGING ***
    // printf("Arguments:\n");
    // for (int i = 0; args[i] != NULL; i++) {
    //     printf("args[%d]: %s\n", i, args[i]);
    // }
    // *** FOR DEBUGGING ***
    return args;    // Return the parsed command as an array of strings
}

// Executing a command
void execute_command(char **args){
    
}

// Parsing and executing the input line
void parse_and_execute(char *input_line){
    // Split the user input line into commands and arguments
    // Multiple commands should be split by a semicolon

    // Execute each command with its corresponding arguments using fork() and exec() family system calls
}

// Executing the 3 built-in commands, maybe we could just do this in parse_and_execute

// Redirection - Ibrahim

// Pipelining

// Signal handling