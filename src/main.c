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
    // AFL++ only fuzzes one argument (!)
    // which is a binary file in our case
    if (argc != 1) 
    {
        printf("No additional arguments are allowed to %s.\n", argv[0]);
        return 1;
    }

    input_context input;
    int input_rv = read_input(&input);

    if (input_rv != 0){
        fprintf(stderr, "Parsing the input (stdio) was not succesfull.\n");
        return 1;
    }

    // set location of next instruction (Regsiter Instruction Pointer)
    char *instruction_address = put_instruction_on_page(
        (size_t)input.offset,
        input.instruction_bytes,
        sizeof(input.instruction_bytes)
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
    int ct_rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%#" PRIx64 "\n", ct_rv, ans);

    xed_answer_t xed_answer;
    memset(&xed_answer, 0, sizeof(xed_answer));

    int xed_rv = disassemble_with_xed(input.instruction_bytes, sizeof(input.instruction_bytes), (uint64_t)instruction_address, &context, &xed_answer);

    if (xed_rv != 0)
    {
        fprintf(stderr, "Error: XED disassembly failed.\n");
        return 1;
    }

    print_xed_answer(&xed_answer, context.rax);

    // int comp_rv = compare_xed_ctd(...);
    // return comp_rv;

    return 0;
}
