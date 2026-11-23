#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

int parse_hex(const char *input, uint64_t *value);
int parse_dec(const char *input, uint64_t *value);

#endif
