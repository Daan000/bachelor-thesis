#ifndef XED_H
#define XED_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint64_t rax;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rflags;
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
    
int compare_xed_ctd(xed_answer_t *xed_answer, uint64_t *ctd_answer, int ct_rv, xed_context *context);

void print_xed_answer(const xed_answer_t *answer, uint64_t rax);

#endif
