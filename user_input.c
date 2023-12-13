#include "shell.h"

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
                        kide_print("Error while reading input.\n");
                        exit(EXIT_FAILURE);
                }
        }
        command[strcspn(command, "\n")] = '\0';
}
