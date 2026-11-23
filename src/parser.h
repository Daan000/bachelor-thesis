#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdint.h>
#include <stddef.h>

int parse_hex_bytes(const char *input, uint8_t *out_bytes, size_t max_bytes, size_t *out_len);
int parse_dec(const char *input, uint64_t *value);

#endif
