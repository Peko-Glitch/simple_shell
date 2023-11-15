#include "simple_shell.h"

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    printf("simple_shell> ");
    fflush(stdout);
}

/**
 * read_command - Reads a command from the user
 *
 * Return: The entered command
 */
char *read_command(void)
{
    char *command = NULL;
    size_t size = 0;

    if (getline(&command, &size, stdin) == -1)
    {
        handle_eof();
    }

    command[strcspn(command, "\n")] = '\0';

    return command;
}

/**
 * execute_command - Executes the given command
 * @command: The command to execute
 */
void execute_command(char *command)
{
    char *const argv[] = {command, NULL};

    if (execve(command, argv, NULL) == -1)
    {
        perror("simple_shell");
        exit(EXIT_FAILURE);
    }
}

/**
 * handle_eof - Handles the end-of-file condition (Ctrl+D)
 */
void handle_eof(void)
{
    printf("\n");
    exit(EXIT_SUCCESS);
}

/**
 * clear_command_buffer - Clears the command buffer
 * @command: The command buffer to clear
 */
void clear_command_buffer(char *command)
{
    memset(command, 0, MAX_COMMAND_LENGTH);
}
