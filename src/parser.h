#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

typedef struct
{
    int offset;
    uint8_t instruction_bytes[15];
} input_context;

int read_input(input_context *value);

#endif
