#include <iostream>
#include <iomanip>
#include <cstdint>

extern "C" {
#include "xed/xed-interface.h"
}

int main() {
    // 1. Initialize XED tables (must be called once at startup)
    xed_tables_init();

    // 2. Configure machine state (64-bit mode, 64-bit address width)
    xed_state_t dstate;
    xed_state_zero(&dstate);
    dstate.mmode = XED_MACHINE_MODE_LONG_64;
    dstate.stack_addr_width = XED_ADDRESS_WIDTH_64b;

    // 3. Prepare the encoder request
    xed_encoder_request_t req;
    xed_encoder_request_zero_set_mode(&req, &dstate);

    // Set instruction class and effective operand width (64-bit for RAX)
    xed_encoder_request_set_iclass(&req, XED_ICLASS_MOV);
    xed_encoder_request_set_effective_operand_width(&req, 64);

    // 4. Configure Operand 0: Destination Register (RAX)
    xed_encoder_request_set_operand_order(&req, 0, XED_OPERAND_REG0);
    xed_encoder_request_set_reg(&req, XED_OPERAND_REG0, XED_REG_RAX);

    // 5. Configure Operand 1: Source Memory ([0xdeadbeef])
    xed_encoder_request_set_operand_order(&req, 1, XED_OPERAND_MEM0);
    xed_encoder_request_set_mem0(&req);
    
    // Set memory access length in bytes (8 bytes = 64 bits)
    xed_encoder_request_set_memory_operand_length(&req, 8);
    
    // Set displacement (32-bit displacement: 0xdeadbeef)
    // Note: In 64-bit mode, a 32-bit displacement without a base register is
    // sign-extended unless an address-size override or SIB byte is used.
    // XED handles the ModRM/SIB encoding automatically.
    xed_encoder_request_set_memory_displacement(&req, 0xdeadbeef, 4); // 4 bytes = 32 bits

    // 6. Encode the instruction
    uint8_t itext[XED_MAX_INSTRUCTION_BYTES];
    unsigned int ilen = 0;
    xed_error_enum_t err = xed_encode(&req, itext, sizeof(itext), &ilen);

    if (err != XED_ERROR_NONE) {
        std::cerr << "Encoding failed: " << xed_error_enum_t2str(err) << "\n";
        return 1;
    }

    // 7. Print resulting machine code bytes
    std::cout << "Encoded bytes (" << ilen << "): ";
    for (unsigned int i = 0; i < ilen; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(itext[i]) << " ";
    }
    std::cout << "\n";

    return 0;
}