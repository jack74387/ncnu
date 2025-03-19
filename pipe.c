#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10 

void parse_command(char *command, char *argv[]) {
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL; // execvp requires NULL termination
}

int main(int argc, char *argv[]) {
    int pipefd[2]; // Pipe file descriptors
    pid_t pid1, pid2;

    if (argc < 3) {  
        fprintf(stderr, "Usage: %s <command1> <command2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) { 
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First fork: Run command1
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) { 
        // Child 1 executes command1
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[1]); // Close write end after redirection

        char *cmd1[MAX_ARGS]; 
        parse_command(argv[1], cmd1); 
        execvp(cmd1[0], cmd1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // **Parent Process run command2
    close(pipefd[1]); // Close unused write end
    waitpid(pid1, NULL, 0); // wait child process

    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
    close(pipefd[0]); // Close unused read end

    char *cmd2[MAX_ARGS]; 
    parse_command(argv[2], cmd2); 
    execvp(cmd2[0], cmd2);
    
    perror("execvp");
    exit(EXIT_FAILURE);

    return 0;
}
