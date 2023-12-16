#include "shell.h"

/**
 * error_126 - Generates an error message for permission denied failures.
 * @args: Array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_126(char **args)
{
	char *error, *hist_str;
	int len;

	/* Convert history number to string */
	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	/* Calculate the length of the error message */
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;

	/* Allocate memory for the error message */
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	/* Build the error message */
	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": Permission denied\n");

	/* Free the history string and return the error message */
	free(hist_str);
	return (error);
}

/**
 * error_127 - Generates an error message for command not found failures.
 * @args: Array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_127(char **args)
{
	char *error, *hist_str;
	int len;

	/* Convert history number to string */
	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	/* Calculate the length of the error message */
	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 16;

	/* Allocate memory for the error message */
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	/* Build the error message */
	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": not found\n");

	/* Free the history string and return the error message */
	free(hist_str);
	return (error);
}

