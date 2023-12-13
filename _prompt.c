#include "shell.h"

void kide_print(const char *format)
{
        write(STDOUT_FILENO, format, strlen(format));
}
