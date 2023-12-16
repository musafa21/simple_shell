#include "shell.h"

/**
 * _copyenv - Creates a copy of the environment.
 *
 * Return: New copy of the environment on success, NULL on failure.
 */
char **_copyenv(void)
{
	char **new_environ;
	size_t size;
	int index;

	/* Count the number of variables in the environment */
	for (size = 0; environ[size]; size++)
		;

	/* Allocate memory for the new environment copy */
	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);

	/* Copy each variable to the new environment */
	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(_strlen(environ[index]) + 1);

		/* Handle memory allocation failure */
		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}

		_strcpy(new_environ[index], environ[index]);
	}
	new_environ[index] = NULL;

	return (new_environ);
}

/**
 * free_env - Frees the environment copy.
 */
void free_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _getenv - Gets an environmental variable from the PATH.
 * @var: Name of the environmental variable to retrieve.
 *
 * Return: Pointer to the variable if it exists, NULL otherwise.
 */
char **_getenv(const char *var)
{
	int index, len;

	len = _strlen(var);

	/* Search for the variable in the environment */
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
