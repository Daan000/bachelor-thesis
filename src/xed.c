#include "xed.h"

#include <inttypes.h>
#include <stdio.h>
#include "xed/xed-interface.h"

void print_xed_answer(const xed_answer_t *answer, uint64_t rax)
{
    if (answer->memop_count == 0)
    {
        printf("no memory operand; RAX=%#" PRIx64 "\n", rax);
        return;
    }

    // maximum 2 iteraties
    for (unsigned int i = 0; i < answer->memop_count; i++)
        printf("xed_memop%u = %#" PRIx64 "\n", i, answer->memop_addresses[i]);
}

// Provides xed the register context
static xed_uint64_t xed_register_callback(xed_reg_enum_t reg, void *context, xed_bool_t *error)
{
    const xed_context *c = (const xed_context *)context;
    *error = c == NULL;

    switch (xed_get_largest_enclosing_register(reg))
    {
    case XED_REG_RAX:
        return c->rax;
    case XED_REG_R8:
        return c->r8;
    case XED_REG_RIP:
        return c->rip;
    default:
        return 0;
    }
}

// Not relevant for us (I think)
static xed_uint64_t xed_segment_callback(xed_reg_enum_t reg, void *context, xed_bool_t *error)
{
    (void)reg;
    (void)context;
    *error = 0;
    return 0;
}

int disassemble_with_xed(const uint8_t *binary_code, size_t code_len, uint64_t runtime_addr,
                         const xed_context *context, xed_answer_t *answer)
{
    answer->memop_count = 0;

    // Initialize XED's internal decode/encode tables (must be called once)
    xed_tables_init();
    xed_agen_register_callback(xed_register_callback, xed_segment_callback);

    // Configure the target machine architecture (64-bit mode, 64-bit address width)
    xed_state_t dstate;
    xed_state_zero(&dstate);
    xed_state_set_machine_mode(&dstate, XED_MACHINE_MODE_LONG_64);
    xed_state_set_stack_address_width(&dstate, XED_ADDRESS_WIDTH_64b);

    char disasm_buf[128];

    xed_decoded_inst_t xedd;
    xed_decoded_inst_zero_set_mode(&xedd, &dstate);

    xed_error_enum_t err = xed_decode(&xedd, binary_code, code_len);
    if (err != XED_ERROR_NONE)
    {
        fprintf(stderr, "Decode error: %s\n", xed_error_enum_t2str(err));
        return 1;
    }

    xed_bool_t ok = xed_format_context(
        XED_SYNTAX_INTEL,
        &xedd,
        disasm_buf,
        sizeof(disasm_buf),
        runtime_addr,
        NULL,
        NULL);

    if (ok)
    {
        printf("xed = %s\n", disasm_buf);
    }
    else
    {
        fprintf(stderr, "Formatting error at 0x%04lx\n", runtime_addr);
        return 1;
    }

    unsigned int memops = xed_decoded_inst_number_of_memory_operands(&xedd);
    answer->memop_count = memops;

    if (memops > 2)
    {
        fprintf(stderr, "Too many XED memory operands: %u\n", memops);
        return 1;
    }

    for (unsigned int memop = 0; memop < memops; memop++)
    {
        xed_uint64_t agen_address = 0;
        xed_error_enum_t agen_error = xed_agen(&xedd, memop, (void *)context, &agen_address);
        if (agen_error != XED_ERROR_NONE)
        {
            fprintf(stderr, "XED address generation failed for memop %u: %s\n",
                    memop, xed_error_enum_t2str(agen_error));
            return 1;
        }

        answer->memop_addresses[memop] = (uint64_t)agen_address;
    }

    return 0;
}