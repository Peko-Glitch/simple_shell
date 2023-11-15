#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

int main()
{
	char command[MAX_COMMAND_LENGTH];

	while (1) {

        printf("simple_shell> ");
        fflush(stdout);

	if (fgets(command, sizeof(command), stdin) == NULL)
	{

		printf("\n");
		break;
	}
	command[strcspn(command, "\n")] = '\0';

	char *const argv[] = { command, NULL };
	if (execve(command, argv, NULL) == -1) {

		perror("simple_shell");
	}


	memset(command, 0, sizeof(command));
	}

	printf("Exiting simple_shell.\n");

	return (0);
}
