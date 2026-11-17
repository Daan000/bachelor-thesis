#include "placement.h"

#include <string.h>

extern char double_page[];

static const size_t DOUBLE_PAGE_SIZE = 2 * 4096;

char *put_instruction_on_page(size_t offset, const void *instruction, size_t len)
{
    // (1) Fill two pages with zeros
    memset(double_page, 0x0, DOUBLE_PAGE_SIZE);

    // (2) Place instruction at offset (with basic bounds check)
    if (offset > DOUBLE_PAGE_SIZE || len > DOUBLE_PAGE_SIZE - offset || (instruction == NULL && len != 0))
    {
        return NULL;
    }
    memcpy(&double_page[offset], instruction, len);

    return &double_page[offset];
}