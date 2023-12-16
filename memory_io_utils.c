#include "shell.h"

/**
 * _realloc - Reallocates memory using malloc and free.
 * @ptr: Pointer to the previously allocated memory.
 * @old_size: Size in bytes of the allocated space for ptr.
 * @new_size: Size in bytes for the new memory block.
 *
 * Return: If new_size == old_size - ptr.
 *	If new_size == 0 and ptr is not NULL - NULL.
 *	Otherwise - Pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	/* If new size equals old size, return the original pointer */
	if (new_size == old_size)
		return (ptr);

	/* If the pointer is NULL, allocate new memory */
	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	/* If new size is 0 and pointer is not NULL, free the memory */
	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	/* Copy the contents from the old block to the new block */
	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * assign_lineptr - Reassigns the lineptr variable for _getline.
 * @lineptr: Buffer to store an input string.
 * @n: Size of lineptr.
 * @buffer: String to assign to lineptr.
 * @b: Size of buffer.
 */
void assign_lineptr(char **lineptr, size_t *n, char *buffer, size_t b)
{
	/* If lineptr is NULL, assign buffer to lineptr */
	if (*lineptr == NULL)
	{
		/* Set n to the size of buffer, or 120 if buffer is smaller */
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	/* If n is less than b, reallocate memory for lineptr and assign buffer */
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*lineptr = buffer;
	}
	/* If n is greater than or equal to b, copy buffer contents to lineptr */
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - Reads input from a stream.
 * @lineptr: Buffer to store the input.
 * @n: Size of lineptr.
 * @stream: Stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	/* If input is not zero, return an error to prevent overlapping calls */
	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		/* If input exceeds 120, reallocate memory for the buffer */
		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	/* Assign buffer to lineptr and handle reallocation if necessary */
	assign_lineptr(lineptr, n, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}
