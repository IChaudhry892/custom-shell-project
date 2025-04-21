# 3600 Major 1 Assignment

## Group Members
- Ibrahim Chaudhry
- Prateek Banda
- Achintya Yalamati

---

## Project Organization

This project implements a custom shell program named `newshell`, supporting interactive and batch modes, built-in commands, pipelining, redirection, and basic signal handling.

### Source Files

- **main.c**: Entry point for the shell; determines whether to run in batch or interactive mode.
- **newshell.h**: Header file with function prototypes and includes for the core shell engine.
- **newshell.c**: Implements core functionality such as input parsing, command execution, redirection pipelining, and mode handlers.
- **builtins.h**: Header file declaring built-in command functions.
- **builtins.c**: Implements built-in commands: `cd`, `exit`, and `path`.
- **makefile**: Compiles all source files and builds the `newshell` executable.

---

## Responsibilities

**Ibrahim Chaudhry:**
- Created initial file structure and `main.c`.
- Wrote core shell engine functions in `newshell.c`:
  - `run_interactive_mode()`
  - `parse_input()`
  - `execute_command()`
  - `parse_and_execute()`
  - `handle_redirection()`
- Declared corresponding prototypes in `newshell.h`.
- Implemented the built-in `cd_command()` in `builtins.c` and its prototype in `builtins.h`.

**Prateek Banda:**
- Implemented `execute_pipeline()` for handling commands with pipes.
- Integrated pipelining in `parse_and_execute()` via `newshell.c`.
- Declared `execute_pipeline()` in `newshell.h`.
- Implemented the built-in `path_command()` in `builtins.c` and declared it in `builtins.h`.

**Achintya Yalamati:**
- Implemented `run_batch_mode()` for processing commands from a file.
- Implemented `handle_signal()` to catch `SIGINT` (Ctrl+C) and `SIGTSTP` (Ctrl+Z).
- Set up signal handling in `run_interactive_mode()` by installing signal handlers.
- Implemented the built-in `exit_command()` in `builtins.c` and its prototype in `builtins.h`.

---

## Design Overview

The shell supports:

- **Interactive Mode**: Prompts user for commands and handles them interactively.
- **Batch Mode**: Reads and executes commands from a batch file, echoing each line before execution.
- **Built-in Commands**: `cd`, `exit`, and `path` are handled internally without forking.
- **External Commands**: All non-built-in commands are executed via `fork()` and `execvp()`.
- **Redirection**: `>` redirects stdout, `<` redirects stdin using `dup2()` and file descriptors.
- **Pipelining**: Supports up to 3 piped commands using `pipe()` and proper process chaining.
- **Signal Handling**: `SIGINT` and `SIGTSTP` signals are caught and handled gracefully in the shell, without terminating it.

Each component is modularized for maintainability and clarity. All functions check return values and handle errors appropriately, ensuring robustness.

---

## Complete Specification & Ambiguities

- **Multiple Commands**: Commands separated by `;` are parsed and run sequentially using `waitpid()` to block the prompt until all finish.
- **Empty Commands**: Lines with nothing or `;;` are silently ignored.
- **Invalid Commands**: An error message is printed and processing continues.
- **Redirection Syntax**: Only supports single `<` or `>` (not both at once or `>>`). No redirection is supported for built-in commands.
- **Pipelining**: Up to 3 commands allowed. Built-in commands are not supported in pipelines.
- **Signal Control**: Signals like Ctrl+C and Ctrl+Z do not kill the shell, only notify. Foreground control and process groups have not been implemented yet.

---

## Known Bugs or Limitations

- **Signal Handling**: `handle_signal()` currently prints a message but does not isolate subprocess signal handling via process groups or `tcsetpgrp()`, which is required by the full spec.
- **Alias Command**: Not implemented (not required for 3-person teams).
- **Command History**: `myhistory` built-in is not implemented (not assigned due to team size).
- **Redirection and Pipelines**: Not yet supported simultaneously (e.g., `cmd1 < in.txt | cmd2 > out.txt` is unsupported).
- **PATH Restoration**: `path_command()` does not restore original environment PATH upon shell exit.

---

## How to Compile and Run
To build the executable using the makefile, run:<br>
`make`

To run the shell in **interactive mode**:<br>
`./bitwisemenu`

To run the shell in **batch mode**:<br>
`./bitwisemenu [batchFile]`

To clean build artifacts:<br>
`make clean`