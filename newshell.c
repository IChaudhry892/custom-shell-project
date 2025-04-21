/*
 * newshell.c - Core implementation of the custom shell engine.
 * Ibrahim Chaudhry, Prateek Banda, Achintya Yalamati - CSCE 3600
 * Implements interactive/batch modes, input parsing, execution logic,
 * redirection, pipelining, and signal handling.
 */
#include "newshell.h"

// Function Implementations For:

// Running the shell in interactive mode
void run_interactive_mode(){ 
    signal(SIGINT, handle_signal);   // Handle Ctrl+C
    signal(SIGTSTP, handle_signal);  // Handle Ctrl+Z

    extern char *original_path;     // Save the original PATH
    original_path = strdup(getenv("PATH"));

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
    // printf("Input line: %s\n", input_line);    // ** FOR DEBUGGING ***

    //if command has '|' handles it by calling the execute_pipeline funciton
    if (strchr(input_line, '|') != NULL) {
        execute_pipeline(input_line);
        return;
    }

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

        // printf("Command %d: %s\n", index+1, command); // *** FOR DEBUGGING ***
        commands[index] = command;
        // *** FOR DEBUGGING ***
        // printf("Commands:\n");
        // for (int i = 0; commands[i] != NULL; i++) {
        //     printf("commands[%d]: %s\n", i, commands[i]);
        // }
        // *** FOR DEBUGGING ***
        command = strtok(NULL, ";");   // Move to the next command
        index++;
    }

    commands[index] = NULL;   // Null-terminate the command list

    // Second loop to execute each command
    for (int i = 0; commands[i] != NULL; i++){
        // Skip leading or trailing whitespace
        while (*commands[i] == ' ' || *commands[i] == '\t') {
            commands[i]++;
        }

        // Ignore empty commands
        if (strlen(commands[i]) == 0){
            continue;
        }

        // printf("Executing command[%d]: %s\n", i, commands[i]);   // *** FOR DEBUGGING ***

        // Duplicate command to avoid strtok state conflicts
        char *command_copy = strdup(commands[i]);
        if (command_copy == NULL) {
            perror("strdup error");
            exit(EXIT_FAILURE);
        }

        // Parse the command into args
        char **args = parse_input(command_copy);

        // Execute command, handle built-in commands
        if (strcmp(args[0], "cd") == 0){
            cd_command(args);
        } else if (strcmp(args[0], "exit") == 0) {
            exit_command();
        } else if (strcmp(args[0], "path") == 0) {
            path_command(args);
        } else if (strchr(commands[i], '<') || strchr(commands[i], '>')){
            handle_redirection(commands[i]);
        } else {
            execute_command(args);
        }

        // Free allocated memory
        free(args);
        free(command_copy);

    }

    free(commands);
}

// Redirection - Ibrahim
void handle_redirection(char *command){
    char *input_file = NULL, *output_file = NULL, *command_part = NULL;

    // Detect redirection, find special characters '<' or '>' in command
    if (strstr(command, "<")){
        command_part = strtok(command, "<");   // command_part = the token before the '<' or '>'
        input_file = strtok(NULL, " \t");      // input/output_file = the token after the '<' or '>'
    } else if (strstr(command, ">")){
        command_part = strtok(command, ">");
        output_file = strtok(NULL, " \t");
    } else{
        fprintf(stderr, "Redirection parsing error\n");
        return;
    }

    // If there's no command or no input/output file
    if (command_part == NULL || (input_file == NULL && output_file == NULL)) {
        fprintf(stderr, "Invalid redirection syntax. Valid syntax: cmd1 < in.txt\tor\tcmd2 > out.txt\n");
        return;
    }

    // Trim whitespace
    while (*command_part == ' ' || *command_part == '\t'){
        command_part++;
    }
    if (input_file){
        while (*input_file == ' ' || *input_file == '\t'){
            input_file++;
        }
    }
    if (output_file){
        while (*output_file == ' ' || *output_file == '\t'){
            output_file++;
        }
    }

    // Tokenize the command part
    char *command_copy = strdup(command_part);
    char **args = parse_input(command_copy);

    pid_t pid = fork();
    if (pid == 0){   // Child process
        // Redirect input if needed
        if (input_file){
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0){
                perror("Input file open error");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, 0); // the file descriptor for stdin is 0
            if (close(fd_in) < 0){
                perror("Input file close error");
                exit(EXIT_FAILURE);
            }
        }

        // Redirect output if needed
        if (output_file){
            int fd_out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644); //0664 -> creates files with permissions rw-r--r--
            if (fd_out < 0){
                perror("Output file open error");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, 1); // the file descriptor for stdout is 1
            if (close(fd_out) < 0){
                perror("Output file close error");
                exit(EXIT_FAILURE);
            }
        }

        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0){   // Parent process
        waitpid(pid, NULL, 0);
    } else{
        perror("fork failed");
    }

    // Free allocated memory
    free(args);
    free(command_copy);
}

// Pipelining - Prateek
void execute_pipeline(char *input_line) {
    char *commands[3];
    int cmd_count = 0;

    char *token = strtok(input_line, "|");
    while (token && cmd_count < 3) {
        while (*token == ' ') token++;  // trim leading spaces
        commands[cmd_count++] = token;
        token = strtok(NULL, "|");
    }

    int pipe_fds[2];
    int prev_fd = -1;

    for (int i = 0; i < cmd_count; i++) {
        char *cmd_copy = strdup(commands[i]);
        char **args = parse_input(cmd_copy);

        if (i < cmd_count - 1) {
            pipe(pipe_fds);
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < cmd_count - 1) {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else {
            if (i > 0) close(prev_fd);
            if (i < cmd_count - 1) {
                close(pipe_fds[1]);
                prev_fd = pipe_fds[0];
            }
            wait(NULL);
        }

        free(args);
        free(cmd_copy);
    }
}


// Signal handling - Achintya
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\nCaught SIGINT (Ctrl+C). Use 'exit' to quit.\nnewshell>> ");
    } else if (sig == SIGTSTP) {
        printf("\nCaught SIGTSTP (Ctrl+Z). Use 'exit' to quit.\nnewshell>> ");
    }
    fflush(stdout);
}
