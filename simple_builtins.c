#include "shell.h"

/**
 * get_builtin - Identifies a given command and associates it
 *	with the corresponding hell builtin function.
 * @command: The command to be identified.
 *
 * Return: A function pointer to the associated builtin function.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", hell_exit },
		{ "env", hell_env },
		{ "setenv", hell_setenv },
		{ "unsetenv", hell_unsetenv },
		{ "cd", hell_cd },
		{ "alias", hell_alias },
		{ "help", hell_help },
		{ NULL, NULL }
	};
	int index;

	for (index = 0; funcs[index].name; index++)
	{
		if (_strcmp(funcs[index].name, command) == 0)
			break;
	}
	return (funcs[index].f);
}

/**
 * hell_exit - Initiates a normal termination process
 *	for the hell shell.
 * @args: An array of arguments including the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If no arguments are provided - -3.
 *	If the specified exit value is invalid - 2.
 *	Otherwise - Exits with the specified status value.
 *
 * Description: Upon returning -3, the program exits back to the main function.
 */
int shellby_exit(char **args, char **front)
{
	int i, num_length = 10;
	unsigned int exit_status = 0, max_exit_status = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			num_length++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= num_length && args[0][i] >= '0' && args[0][i] <= '9')
				exit_status = (exit_status * 10) + (args[0][i] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (exit_status > max_exit_status - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(exit_status);
}

/**
 * hell_cd - Modifies the current directory of the hell process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If the specified string is not a directory - 2.
 *	If an error occurs - -1.
 *	Otherwise - 0.
 */
int hell_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info, *new_line = "\n";
	char *old_directory = NULL, *current_directory = NULL;
	struct stat directory_stat;

	old_directory = getcwd(old_directory, 0);
	if (!old_directory)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_getenv("OLDPWD") != NULL)
					(chdir(*_getenv("OLDPWD") + 7));
			}
			else
			{
				free(old_directory);
				return (create_error(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &directory_stat) == 0 && S_ISDIR(directory_stat.st_mode)
					&& ((directory_stat.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(old_directory);
				return (create_error(args, 2));
			}
		}
	}
	else
	{
		if (_getenv("HOME") != NULL)
			chdir(*(_getenv("HOME")) + 5);
	}

	current_directory = getcwd(current_directory, 0);
	if (!current_directory)
		return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
		return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = old_directory;
	if (hell_setenv(dir_info, dir_info) == -1)
		return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = current_directory;
	if (hell_setenv(dir_info, dir_info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, current_directory, _strlen(current_directory));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(old_directory);
	free(current_directory);
	free(dir_info);
	return (0);
}

/**
 * hell_help - Presents information about hell builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *	Otherwise - 0.
 */
int hell_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
