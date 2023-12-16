#include "shell.h"

/**
 * sig_handler - Handles signals and prints a new prompt.
 * @sig: The signal number.
 */
void sig_handler(int sig)
{
	/* Define a new prompt for the shell. */
	char *new_prompt = "\n$ ";

	(void)sig;
	/* Reset the signal handler to sig_handler. */
	signal(SIGINT, sig_handler);
	/* Write the new prompt to the standard output. */
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * execute - Executes a command in a child process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - a corresponding error code.
 *	Otherwise - The exit value of the last executed command.
 */
int execute(char **args, char **front)
{
	/* Process ID for the child process. */
	pid_t child_pid;
	/* Status of the child process. */
	int status, flag = 0, ret = 0;
	/* The command to be executed. */
	char *command = args[0];

	/* Check if the command is a relative or absolute path. */
	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		/* Get the location of the command. */
		command = get_location(command);
	}

	/* Check if the command exists and can be executed. */
	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (create_error(args, 126));
		else
			ret = (create_error(args, 127));
	}
	else
	{
		/* Fork a new process for command execution. */
		child_pid = fork();
		if (child_pid == -1)
		{
			/* Handle fork error. */
			if (flag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			/* Execute the command in the child process. */
			execve(command, args, environ);
			/* Handle execution error. */
			if (errno == EACCES)
				ret = (create_error(args, 126));
			free_env();
			free_args(args, front);
			_exit(ret);
		}
		else
		{
			/* Wait for the child process to finish. */
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}

	/* Free allocated memory if necessary. */
	if (flag)
		free(command);
	return (ret);
}

/**
 * main - Runs a simple UNIX command interpreter.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: The return value of the last executed command.
 */
int main(int argc, char *argv[])
{
	/* Return values for the main function. */
	int ret = 0, retn;
	int *exe_ret = &retn;
	/* Strings for the shell prompt. */
	char *prompt = "$ ", *new_line = "\n";

	/* Set the program name and initialize variables. */
	name = argv[0];
	hist = 1;
	aliases = NULL;
	/* Set the signal handler for SIGINT. */
	signal(SIGINT, sig_handler);

	/* Initialize return value. */
	*exe_ret = 0;
	/* Copy the environment variables. */
	environ = _copyenv();
	/* Exit if environment copy fails. */
	if (!environ)
		exit(-100);

	/* Check if there is a command-line argument. */
	if (argc != 1)
	{
		/* Process commands from a file. */
		ret = proc_file_commands(argv[1], exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	/* Check if standard input is not a terminal. */
	if (!isatty(STDIN_FILENO))
	{
		/* Process commands from non-interactive input. */
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_args(exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}

	/* Main loop for interactive shell. */
	while (1)
	{
		/* Print the shell prompt. */
		write(STDOUT_FILENO, prompt, 2);
		/* Handle user input and execute commands. */
		ret = handle_args(exe_ret);
		/* Check for end-of-file or exit command. */
		if (ret == END_OF_FILE || ret == EXIT)
		{
			/* Print a new line before exiting if end-of-file. */
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			free_alias_list(aliases);
			exit(*exe_ret);
		}
	}

	/* Free allocated memory before exiting. */
	free_env();
	free_alias_list(aliases);
	return (*exe_ret);
}

