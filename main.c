#include "simple_shell.h"

int main(void)
{
    char *command;

    while (1)
    {
        display_prompt();
        command = read_command();
        execute_command(command);
        clear_command_buffer(command);
    }

    return 0;
}

