#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

void display_prompt(void);
char *read_command(void);
void execute_command(char *command);
void handle_eof(void);
void clear_command_buffer(char *command);

#endif
