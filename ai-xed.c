#include <stdio.h>
#include <stdint.h>
#include "xed/xed-interface.h"

int main(void) {
    // 1. Initialize XED's internal decode/encode tables (must be called once)
    xed_tables_init();

    // 2. Configure the target machine architecture (64-bit mode, 64-bit address width)
    xed_state_t dstate;
    xed_state_zero(&dstate);
    xed_state_set_machine_mode(&dstate, XED_MACHINE_MODE_LONG_64);
    xed_state_set_stack_address_width(&dstate, XED_ADDRESS_WIDTH_64b);

    // Sample raw machine code bytes:
    // 55                      -> push rbp
    // 48 89 e5                -> mov rbp, rsp
    // 48 83 ec 10             -> sub rsp, 0x10
    // c3                      -> ret
    uint8_t binary_code[] = { 0x41, 0x89, 0x48, 0x3c};
    size_t code_len = sizeof(binary_code);
    size_t offset = 0;
    uint64_t runtime_addr = 0x1000; // Simulated starting memory address

    char disasm_buf[128];

    // 3. Loop over the binary buffer instruction by instruction
    while (offset < code_len) {
        xed_decoded_inst_t xedd;
        xed_decoded_inst_zero_set_mode(&xedd, &dstate);

        // Decode one instruction from the current offset
        xed_error_enum_t err = xed_decode(
            &xedd,
            binary_code + offset,
            code_len - offset
        );

        if (err != XED_ERROR_NONE) {
            fprintf(stderr, "Decode error at offset %zu: %s\n",
                    offset, xed_error_enum_t2str(err));
            break;
        }

        // 4. Format the decoded instruction into Intel assembly syntax
        xed_bool_t ok = xed_format_context(
            XED_SYNTAX_INTEL,
            &xedd,
            disasm_buf,
            sizeof(disasm_buf),
            runtime_addr,
            NULL,
            NULL
        );

        if (ok) {
            printf("0x%04lx:  %s\n", runtime_addr, disasm_buf);
        } else {
            fprintf(stderr, "Formatting error at 0x%04lx\n", runtime_addr);
        }

        // Advance offset and address by the exact byte length of the decoded instruction
        uint32_t inst_len = xed_decoded_inst_get_length(&xedd);
        offset += inst_len;
        runtime_addr += inst_len;
    }

    return 0;
}