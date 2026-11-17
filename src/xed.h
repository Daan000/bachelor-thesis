#ifndef XED_H
#define XED_H

#include <stddef.h>
#include <stdint.h>

int disassemble_with_xed(const uint8_t *binary_code, size_t code_len, uint64_t runtime_addr);

#endif
