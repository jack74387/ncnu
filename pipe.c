#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2]; // Pipe file descriptors
    pid_t pid;

    if (pipe(pipefd) == -1) { // Create pipe
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // Child process executes `wc -l`
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipefd[0]); // Close original read end
        execlp("wc", "wc", "-l", NULL); // Execute `wc -l`
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process executes `ls`
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[1]); // Close original write end
        execlp("ls", "ls", NULL); // Execute `ls`
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}
