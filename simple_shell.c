#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

void display_prompt() {
    printf("$ ");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char* args[2];

    while (1) {
        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;  // Handle Ctrl+D (end of file)
        }

        // Remove the trailing newline character
        input[strcspn(input, "\n")] = '\0';

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            args[0] = input;
            args[1] = NULL;

            if (execve(args[0], args, NULL) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
