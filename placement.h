#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <stddef.h>

char *put_instruction_on_page(size_t offset, const void *instruction, size_t len);

#endif
