#include <stdint.h>
#include <string.h>

extern char double_page[];

char *put_instruction_on_page(int offset, const void *instruction, size_t len)
/*
offset:
instruction:
len: bytes of instruction (1-15)
*/
{
    // (1) Fill two pages with zeros
    memset(double_page, 0x0, 2 * 4096);

    // (2) Place instruction at offset (with basic bounds check)
    if (offset >= 0 && (size_t)offset + len <= 2 * 4096) {
        memcpy(&double_page[offset], instruction, len);
    }

    return &double_page[offset];
}