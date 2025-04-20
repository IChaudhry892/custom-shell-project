#include "newshell.h"
void handle_sigint(int sig) {
    printf("\nCaught signal %d (SIGINT). Use 'exit' to quit.\nnewshell>> ", sig);
    fflush(stdout);
}


// Function Implementations For:

// Running the shell in interactive mode
void run_interactive_mode(){ 
    signal(SIGINT, handle_sigint);  // Register SIGINT handler
    char input_line[MAX_COMMAND_INPUT_SIZE];    // Character array to store user input
    while (1){
        // Prompt user for input
        printf("newshell>> ");
        if (fgets(input_line, MAX_COMMAND_INPUT_SIZE, stdin) == NULL){  // Gets user input
            // handle error or end of file
            break;
        }
        input_line[strcspn(input_line, "\n")] = '\0';   // Remove the newline character at the end of fgets() input

        if (strlen(input_line) == 0){   // Ignore empty user input, re-prompt
            continue;
        }

        // *** FOR DEBUGGING ***
        // printf("%s\n", input_line);
        // char **args = parse_input(input_line);
        // *** FOR DEBUGGING ***

        parse_and_execute(input_line);
    }
}

// Running the shell in batch mode
void run_batch_mode(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Batch file open error");
        exit(EXIT_FAILURE);
    }

    char input_line[MAX_COMMAND_INPUT_SIZE];
    while (fgets(input_line, MAX_COMMAND_INPUT_SIZE, file) != NULL) {
        input_line[strcspn(input_line, "\n")] = '\0'; // Remove newline
        if (strlen(input_line) > 0) {
            parse_and_execute(input_line);
        }
    }

    fclose(file);
}


// Tokenizing a command
char **parse_input(char *command){
    int buffer_size = 64; // Initial buffer for **args to hold up to 64 arguments

    char **args = malloc(buffer_size * sizeof(char *)); // Allocates memory for an array of buffer_size string pointers to hold parsed arguments (e.g., "ls", "-l", NULL)
    if (args == NULL){  // If memory allocation failed
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    int index = 0;  // Keeps track of the number of arguments passed so far
    char *token = strtok(command, " \t");   // Splits the command string at spaces or tabs

    while (token != NULL){
        if (index >= buffer_size){ // If the number of arguments exceeds the initial buffer size;
            buffer_size *= 2;   // Double the buffer size
            args = realloc(args, buffer_size * sizeof(char *));
            if (args == NULL){  // If memory reallocation failed
                perror("realloc error");
                exit(EXIT_FAILURE);
            }
        }

        printf("Token %d: %s\n", index+1, token); // *** FOR DEBUGGING ***
        args[index] = token;    // Add current token to the args array
        token = strtok(NULL, " \t");    // Move to the next token
        index++;
    }

    args[index] = NULL; // Last argument should be NULL for args to work with execvp()
    // *** FOR DEBUGGING ***
    printf("Arguments:\n");
    for (int i = 0; args[i] != NULL; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }
    // *** FOR DEBUGGING ***
    return args;    // Return the parsed command as an array of strings
}

// Executing a command
void execute_command(char **args){
    if (args[0] == NULL){   // Ignore empty (NULL) commands
        return;
    }

    pid_t pid = fork();
    
    if (pid == 0){   // Child process
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else if (pid > 0){   // Parent Process
        waitpid(pid, NULL, 0);  // Wait for the child to finish
    } else{   // Error
        perror("fork error");
    }
}

// Parsing and executing the input line
void parse_and_execute(char *input_line){
    // Split the user input line into multiple commands by semicolons
    printf("Input line: %s\n", input_line);    // ** FOR DEBUGGING ***

    int buffer_size = 64; // Initial buffer for **commands to hold up to 64 commands
    char **commands = malloc(buffer_size * sizeof(char *)); // Allocates memory for an array of buffer_size string pointers to hold parsed commands
    if (commands == NULL){  // If memory allocation failed
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    int index = 0;  // Keeps track of the number of commands    
    char *command = strtok(input_line, ";");    // Split the input line at semicolons

    while (command != NULL){
        if (index >= buffer_size){ // If the number of commands exceeds the initial buffer size;
            buffer_size *= 2;   // Double the buffer size
            commands = realloc(commands, buffer_size * sizeof(char *));
            if (commands == NULL){  // If memory reallocation failed
                perror("realloc error");
                exit(EXIT_FAILURE);
            }
        }

        printf("Command %d: %s\n", index+1, command); // *** FOR DEBUGGING ***
        commands[index] = command;
        // *** FOR DEBUGGING ***
        printf("Commands:\n");
        for (int i = 0; commands[i] != NULL; i++) {
            printf("commands[%d]: %s\n", i, commands[i]);
        }
        // *** FOR DEBUGGING ***

        // while (*command == ' ' || *command =='\t'){   // Skip leading or trailing whitespace
        //     command++;
        // }

        // if (strlen(command) == 0){   // Ignore empty commands
        //     command = strtok(NULL, ";");
        //     continue;
        // }

        // Parse the command into args
        // char **args = parse_input(command);

        // Execute the command
        // execute_command(args);

        // free(args);   // Free the memory allocated by parse_input(command)
        command = strtok(NULL, ";");   // Move to the next command
        index++;
    }

    commands[index] = NULL;   // Null-terminate the command list

    // Second loop to execute each command
    for (int i = 0; commands[i] != NULL; i++){
        // Trip leading or trailing whitespace
        while (*commands[i] == ' ' || *commands[i] == '\t') {
            commands[i]++;
        }

        // Ignore empty commands
        if (strlen(commands[i]) == 0){
            continue;
        }

        printf("Executing command[%d]: %s\n", i, commands[i]);   // *** FOR DEBUGGING ***

        // Duplicate command to avoid strtok state conflicts
        char *command_copy = strdup(commands[i]);
        if (command_copy == NULL) {
            perror("strdup error");
            exit(EXIT_FAILURE);
        }

        char **args = parse_input(command_copy);

        // Handle built-in commands
        if (strcmp(args[0], "exit") == 0) {
            exit_command();
        } else if (strcmp(args[0], "path") == 0) {
            path_command();
        } else {
            execute_command(args);
        }

        free(args);
        free(command_copy);

    }

    free(commands);
}

// Executing the 3 built-in commands, maybe we could just do this in parse_and_execute

// Redirection - Ibrahim

// Pipelining

// Signal handling
