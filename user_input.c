#include "shell.h"

/**
 * get_input_command - Get user input for the command
 * @command: Buffer to store the command
 * @size: Size of the buffer
 *
 * Return: No return value
 */
void get_input_command(char *command, size_t size)
{
	if (fgets(command, size, stdin) == NULL)
	{
		if (feof(stdin))
		{
			_print("\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			_print("Error while reading input.\n");
			exit(EXIT_FAILURE);
		}
	}
	command[strcspn(command, "\n")] = '\0';
}
