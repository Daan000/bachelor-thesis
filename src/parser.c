#include "parser.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>


int read_input_from_buf(input_context *input, const uint8_t *buf, size_t len)
{
    if (input == NULL || buf == NULL || len == 0) {
        return -1;
    }

    memset(input, 0, sizeof(*input));

    size_t copy_len = len < sizeof(*input) ? len : sizeof(*input);
    memcpy(input, buf, copy_len);

    return 0;
}

int parse_dec(const char *input, uint64_t *value)
{
    char *endptr = NULL;
    unsigned long long parsed;

    if (input == NULL || value == NULL)
    {
        fprintf(stderr, "Error: Null argument.\n");
        return -1;
    }

    // Base 10: the offset is decimal, "0x" prefixes are rejected
    errno = 0;
    parsed = strtoull(input, &endptr, 10);

    // Check if no digits were found at all (e.g., "")
    if (endptr == input)
    {
        fprintf(stderr, "Error: No valid decimal digits found.\n");
        return -1;
    }

    // Check if trailing garbage characters exist (e.g., "100x")
    if (*endptr != '\0')
    {
        fprintf(stderr, "Error: Invalid character '%c' in input.\n", *endptr);
        return -1;
    }

    if (errno == ERANGE || parsed > UINT64_MAX)
    {
        fprintf(stderr, "Error: Decimal value exceeds 64-bit limit.\n");
        return -1;
    }

    *value = (uint64_t)parsed;
    return 0;
}
