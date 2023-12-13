#include "shell.h"

/**
 * kide_print - Custom print function
 * @format: The format string to be printed
 *
 * Return: No return value
 */
void kide_print(const char *format)
{
	write(STDOUT_FILENO, format, strlen(format));
}
