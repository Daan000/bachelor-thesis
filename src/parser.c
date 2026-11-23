#include "parser.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

int read_input(input_context *value)
{
    memset(value, 0, sizeof(value));

    ssize_t len = read(STDIN_FILENO, value, sizeof(value));
    if (len <= 0) return -1;

    return 0;
}