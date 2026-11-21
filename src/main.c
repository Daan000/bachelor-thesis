#include "ctd/ctd.h"
#include "xed.h"
#include "parser.h"
#include "placement.h"

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

static void print_xed_answer(const xed_answer_t *answer, uint64_t rax)
{
    if (answer->memop_count == 0)
    {
        printf("no memory operand; RAX=%#" PRIx64 "\n", rax);
        return;
    }

    for (unsigned int i = 0; i < answer->memop_count; i++)
        printf("xed_memop%u = %#" PRIx64 "\n", i, answer->memop_addresses[i]);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <offset> <instruction>\n", argv[0]);
        return 1;
    }

    uint64_t offset;
    uint64_t instruction;
    uint8_t instruction_bytes[15] = {0};

    if (parse_hex(argv[1], &offset) != 0 || parse_hex(argv[2], &instruction) != 0)
    {
        return 1;
    }

    memcpy(instruction_bytes, &instruction, sizeof(instruction));

    if (offset > SIZE_MAX)
    {
        fprintf(stderr, "Error: Offset is too large for this platform.\n");
        return 1;
    }

    // Create sgx context
    sgx_cpu_context_t sgx;
    memset(&sgx, 0x0, sizeof(sgx)); // set all registers to 0

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

    sgx.rip = (uint64_t)instruction_address;
    sgx.rax = 0xdeadbeefcafebabe;
    sgx.r8 = 0x0123456789abcdef;

    uint64_t ans = 0;
    int rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%#" PRIx64 "\n", rv, ans);

    xed_context context;
    context.rax = sgx.rax;
    context.r8 = sgx.r8;
    context.rip = sgx.rip;

    xed_answer_t xed_answer;
    xed_answer.memop_count = 0;
    xed_answer.memop_addresses[0] = 0;
    xed_answer.memop_addresses[1] = 0;

    int xed_rv = disassemble_with_xed(instruction_bytes, sizeof(instruction), (uint64_t)instruction_address, &context, &xed_answer);
    printf("xed_rv = %d\n", xed_rv);
    if (xed_rv != 0)
    {
        fprintf(stderr, "Error: XED disassembly failed.\n");
        return 1;
    }

    print_xed_answer(&xed_answer, context.rax);

    return 0;
}
