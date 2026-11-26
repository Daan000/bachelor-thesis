#include "xed.h"

#include <inttypes.h>
#include <stdio.h>
#include "xed/xed-interface.h"

// Compare the CTD result with the trusted XED result.
// `ct_rv` is essential here: CTD's `addr` output is only meaningful when
// the decoder actually found a memory access (rv != 0). For instructions
// without a memory operand, `addr` is derived from the bytes following the
// opcode and must not be compared against XED's RAX fallback.
int compare_xed_ctd(xed_answer_t *xed_answer, uint64_t *ctd_answer, int ct_rv, xed_context *context){
    if (xed_answer == NULL || ctd_answer == NULL || context == NULL) return -1;

    int ctd_has_mem = (ct_rv != 0);
    int xed_has_mem = (xed_answer->memop_count > 0);

    // First the decoders must agree on whether there is a memory access at all.
    if (ctd_has_mem != xed_has_mem) return -1;

    // Neither decoder found a memory operand: there is no address to compare.
    if (!ctd_has_mem) return 0;

    // Both found one: XED is the trusted source for the effective address.
    if (*ctd_answer == xed_answer->memop_addresses[0]) return 0; // ALL OK.
    return -1; // NOT OK, CTD was not equal to the trusted source XED
}

void print_xed_answer(const xed_answer_t *answer, uint64_t rax)
{
    if (answer->memop_count == 0)
    {
        printf("[XED] no memory operand; RAX=%#" PRIx64 "\n", rax);
        return;
    }

    // maximum 2 iteraties (anders zou `disassemble_with_xed` falen)
    for (unsigned int i = 0; i < answer->memop_count; i++)
        printf("[XED] xed_memop%u = %#" PRIx64 "\n", i, answer->memop_addresses[i]); // e.g. `xed_memop0 = 0x123456789abcdef`
}


static xed_uint64_t xed_segment_callback(xed_reg_enum_t reg, void *context, xed_bool_t *error)
{
    (void)reg;
    (void)context;
    *error = 0;
    return 0;
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

int disassemble_with_xed(const uint8_t *binary_code, size_t code_len, uint64_t runtime_addr,
                         const xed_context *context, xed_answer_t *answer)
{
    answer->memop_count = 0;

    // Initialize XED's internal decode/encode tables (must be called once)
    xed_tables_init();
    // Configure the target machine architecture (64-bit mode, 64-bit address width)
    xed_state_t dstate;
    xed_state_zero(&dstate);
    xed_state_set_machine_mode(&dstate, XED_MACHINE_MODE_LONG_64);
    xed_state_set_stack_address_width(&dstate, XED_ADDRESS_WIDTH_64b);
    xed_agen_register_callback(xed_register_callback, xed_segment_callback);

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
        // print the disassembled instruction (e.g. `add byte ptr [r8], al`)
        printf("[XED] i = %s\n", disasm_buf); 
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

    // for all memory operands, store the address
    // e.g. `xed_memop0 = 0x123456789abcdef`
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