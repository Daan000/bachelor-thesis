#ifndef XED_H
#define XED_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    // Dit zijn momenteel de enigste registers die we instellen
    uint64_t rax;
    uint64_t r8;
    uint64_t rip;
} xed_context;

typedef struct
{
    unsigned int memop_count;    // hoeveel memory operands er in een instructie aanwezig zijn
    uint64_t memop_addresses[2]; // de adressen die bij de vorige memory operands horen
                                 // de array heeft lengte 2 doordat dat het maximum aan memory operands is
} xed_answer_t;

int disassemble_with_xed(const uint8_t *binary_code, size_t code_len, uint64_t runtime_addr,
                         const xed_context *context, xed_answer_t *answer);

#endif
