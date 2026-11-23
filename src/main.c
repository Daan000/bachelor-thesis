#include "ctd/ctd.h"
#include "xed.h"
#include "parser.h"
#include "placement.h"
#include "register_fill.h"

#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

int g_cpu_feature_indicator = 0x0;

void dump_hex(uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
        printf("%02x", *(buf + i));
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <decimal offset> <hex instruction>\n", argv[0]);
        return 1;
    }

    uint64_t offset;
    uint64_t instruction;
    uint8_t instruction_bytes[15] = {0};

    // The offset is decimal, the instruction bytes are hexadecimal
    if (parse_dec(argv[1], &offset) != 0 || parse_hex(argv[2], &instruction) != 0)
    {
        return 1;
    }

    memcpy(instruction_bytes, &instruction, sizeof(instruction));

    
    // set location of next instruction (Regsiter Instruction Pointer)
    char *instruction_address = put_instruction_on_page(
        (size_t)offset,
        instruction_bytes,
        sizeof(instruction_bytes)
    );
    
    if (instruction_address == NULL)
    {
        fprintf(stderr, "Error: Instruction does not fit on the page.\n");
        return 1;
    }
    
    // Create sgx context
    sgx_cpu_context_t sgx;
    fill_registers(&sgx, (uint64_t)instruction_address);
    
    // Copy the registers for later use by XED
    xed_context context;
    copy_registers(&sgx, &context);

    uint64_t ans = 0;
    int rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%#" PRIx64 "\n", rv, ans);

    xed_answer_t xed_answer;
    memset(&xed_answer, 0x0, sizeof(xed_answer_t));

    int xed_rv = disassemble_with_xed(instruction_bytes, sizeof(instruction), (uint64_t)instruction_address, &context, &xed_answer);

    if (xed_rv != 0)
    {
        fprintf(stderr, "Error: XED disassembly failed.\n");
        return 1;
    }

    print_xed_answer(&xed_answer, context.rax);

    return 0;
}
