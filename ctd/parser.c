#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int parse_hex(char *input)
{
    char *endptr = NULL;

    // Base 16 parses both "0xdeadbeefcafebabe" and "deadbeefcafebabe"
    unsigned long long parsed = strtoull(input, &endptr, 16);

    // 1. Check if no digits were found at all (e.g., "" or "0x")
    if (endptr == input) {
        fprintf(stderr, "Error: No valid hexadecimal digits found.\n");
        exit(0);
    }

    // 2. Check if trailing garbage characters exist (e.g., "0x1234xyz")
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid character '%c' in input.\n", *endptr);
        exit(0);
    }

    // 3. Check for overflow (> 0xffffffffffffffff / 16 hex digits)
    // if (errno == ERANGE || parsed > UINT64_MAX) {
    //     fprintf(stderr, "Error: Hex value exceeds 64-bit limit.\n");
    //     exit(0);
    // }

    // Print using portable inttypes.h macros
    // printf("Parsed Hex: 0x%" PRIx64 "\n", value);
    // printf("Decimal:    %" PRIu64 "\n", value);
    return parsed;
}