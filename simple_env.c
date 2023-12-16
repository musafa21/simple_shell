#include "shell.h"

/**
 * hell_env - Prints info about the 'env' prototype.
 * @args: Array of arguments passed to the shell.
 * @front: Double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 * Description: Prints variables in 'variable'='value' format.
 */
int hell_env(char **args, char __attribute__((__unused__)) **front)
{
	int i;
	char nl = '\n';

	if (!environ)
		return (-1);

	for (i = 0; environ[i]; i++) {
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, &nl, 1);
	}

	(void)args;
	return (0);
}

/**
 * hell_setenv - Prints info about the 'setenv' prototype.
 * @args: Array of arguments passed to the shell.
 * @front: Double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 * Description: args[1] is the name of the variable, args[2] is its value.
 */
int hell_setenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var = NULL, **new_environ, *new_value;
	size_t size;
	int i;

	if (!args[0] || !args[1])
		return (create_error(args, -1));

	new_value = malloc(_strlen(args[0]) + 1 + _strlen(args[1]) + 1);
	if (!new_value)
		return (create_error(args, -1));

	_strcpy(new_value, args[0]);
	_strcat(new_value, "=");
	_strcat(new_value, args[1]);

	env_var = _getenv(args[0]);
	if (env_var) {
		free(*env_var);
		*env_var = new_value;
		return (0);
	}

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 2));
	if (!new_environ) {
		free(new_value);
		return (create_error(args, -1));
	}

	for (i = 0; environ[i]; i++)
		new_environ[i] = environ[i];

	free(environ);
	environ = new_environ;
	environ[i] = new_value;
	environ[i + 1] = NULL;

	return (0);
}

/**
 * hell_unsetenv - Prints info about the 'unsetenv' prototype.
 * @args: Array of arguments passed to the shell.
 * @front: Double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 * Description: args[1] is the variable to remove.
 */
int hell_unsetenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var, **new_environ;
	size_t size;
	int i, j;

	if (!args[0])
		return (create_error(args, -1));

	env_var = _getenv(args[0]);
	if (!env_var)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * size);
	if (!new_environ)
		return (create_error(args, -1));

	for (i = 0, j = 0; environ[i]; i++) {
		if (*env_var == environ[i]) {
			free(*env_var);
			continue;
		}
		new_environ[j] = environ[i];
		j++;
	}

	free(environ);
	environ = new_environ;
	environ[size - 1] = NULL;

	return (0);
}
