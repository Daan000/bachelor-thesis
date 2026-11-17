#include "parser.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int parse_hex(const char *input, uint64_t *value)
{
    char *endptr = NULL;
    unsigned long long parsed;

    if (input == NULL || value == NULL) {
        fprintf(stderr, "Error: Null argument.\n");
        return -1;
    }

    // Base 16 parses both "0xdeadbeefcafebabe" and "deadbeefcafebabe"
    errno = 0;
    parsed = strtoull(input, &endptr, 16);

    // 1. Check if no digits were found at all (e.g., "" or "0x")
    if (endptr == input) {
        fprintf(stderr, "Error: No valid hexadecimal digits found.\n");
        return -1;
    }

    // 2. Check if trailing garbage characters exist (e.g., "0x1234xyz")
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid character '%c' in input.\n", *endptr);
        return -1;
    }

    // 3. Check for overflow (> 0xffffffffffffffff / 16 hex digits)
    if (errno == ERANGE || parsed > UINT64_MAX) {
        fprintf(stderr, "Error: Hex value exceeds 64-bit limit.\n");
        return -1;
    }

    *value = (uint64_t)parsed;
    return 0;
}