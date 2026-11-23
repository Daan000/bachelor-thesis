#include "parser.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

// Parses a hex string (e.g., "488b0425efbeadde", "0x488b...", or "48 8b 04...")
// into a byte array in instruction stream order.
int parse_hex_bytes(const char *input, uint8_t *out_bytes, size_t max_bytes, size_t *out_len)
{
    if (input == NULL || out_bytes == NULL || out_len == NULL)
    {
        fprintf(stderr, "Error: Null argument.\n");
        return -1;
    }

    const char *p = input;

    // Skip leading whitespace
    while (isspace((unsigned char)*p))
    {
        p++;
    }

    // Skip optional "0x" or "0X" prefix
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
    {
        p += 2;
    }

    size_t count = 0;

    while (*p != '\0')
    {
        // Allow optional whitespace between bytes (e.g. "48 8b 04")
        if (isspace((unsigned char)*p))
        {
            p++;
            continue;
        }

        if (!isxdigit((unsigned char)p[0]))
        {
            fprintf(stderr, "Error: Invalid character '%c' in input.\n", *p);
            return -1;
        }

        if (p[1] == '\0' || !isxdigit((unsigned char)p[1]))
        {
            fprintf(stderr, "Error: Incomplete hex byte at '%c'. Each byte requires 2 hex digits.\n", p[0]);
            return -1;
        }

        if (count >= max_bytes)
        {
            fprintf(stderr, "Error: Instruction exceeds maximum length of %zu bytes.\n", max_bytes);
            return -1;
        }

        char byte_str[3] = { p[0], p[1], '\0' };
        out_bytes[count++] = (uint8_t)strtoul(byte_str, NULL, 16);
        p += 2;
    }

    if (count == 0)
    {
        fprintf(stderr, "Error: No valid hexadecimal bytes found.\n");
        return -1;
    }

    *out_len = count;
  
  return 0;
}

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
