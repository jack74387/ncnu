# NCNU
## OS class
### Unix Pipe Implementation
#### Overview

This C program demonstrates how to execute two commands in a pipeline using pipe(), fork(), dup2(), and execvp(). The first command's output is redirected into a pipe, and the second command reads from that pipe as input.

#### Compilation

To compile the program, use the following command:
```bash
gcc -o pipe_exec pipe_exec.c
```
#### Usage

Run the program with two commands as arguments. The first command will write to the pipe, and the second command will read from it.
```bash
./pipe_exec "command1" "command2"
```
- Example Usage
```bash
./pipe_exec "ls -l" "less"
```
    This will execute ls -l and send the output to less.

