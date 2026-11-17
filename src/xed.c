#include "xed.h"

#include <stdio.h>
#include "xed/xed-interface.h"

int disassemble_with_xed(const uint8_t *binary_code, size_t code_len, uint64_t runtime_addr)
{
    // 1. Initialize XED's internal decode/encode tables (must be called once)
    xed_tables_init();

    // 2. Configure the target machine architecture (64-bit mode, 64-bit address width)
    xed_state_t dstate;
    xed_state_zero(&dstate);
    xed_state_set_machine_mode(&dstate, XED_MACHINE_MODE_LONG_64);
    xed_state_set_stack_address_width(&dstate, XED_ADDRESS_WIDTH_64b);

    size_t offset = 0;

    char disasm_buf[128];

    // 3. Loop over the binary buffer instruction by instruction
    while (offset < code_len)
    {
        xed_decoded_inst_t xedd;
        xed_decoded_inst_zero_set_mode(&xedd, &dstate);

        // Decode one instruction from the current offset
        xed_error_enum_t err = xed_decode(
            &xedd,
            binary_code + offset,
            code_len - offset);

        if (err != XED_ERROR_NONE)
        {
            fprintf(stderr, "Decode error at offset %zu: %s\n",
                    offset, xed_error_enum_t2str(err));
            return 1;
        }

        // 4. Format the decoded instruction into Intel assembly syntax
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

        break;
    }

    return 0;
}