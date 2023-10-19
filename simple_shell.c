#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

int main(void)
{
    char *command = NULL;
    size_t command_length = 0;
    char *args[2];
    char *envp[] = { NULL };

    while (1)
    {
        /* Display prompt and wait for user input */
        write(1, "$ ", 2);
        fsync(1);

        /* Read user input */
        if (getline(&command, &command_length, stdin) == -1)
        {
            free(command);
            write(1, "\n", 1);
            break; /* Handle end of file (Ctrl+D) */
        }

        /* Remove trailing newline character */
        command[strcspn(command, "\n")] = 0;

        /* Check if executable exists */
        if (access(command, X_OK) != 0)
        {
            write(2, "Error: Command not found\n", 25);
            continue;
        }

        /* Execute command */
        args[0] = command;
        args[1] = NULL;
        if (fork() == 0)
        {
            execve(command, args, envp);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL); /* Wait for child process to finish */
        }
    }

    free(command);
    return 0;
}
