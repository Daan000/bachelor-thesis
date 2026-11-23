#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    int offset;
    uint8_t instruction_bytes[15];
} input_context;

int read_input_from_buf(input_context *input, const uint8_t *buf, size_t len);

int parse_hex(const char *input, uint64_t *value);
int parse_dec(const char *input, uint64_t *value);

#endif
