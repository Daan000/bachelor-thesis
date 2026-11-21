#include "ctd/ctd.h"
#include "parser.h"
#include "placement.h"
// #include "xed-decoder.c"

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

    int rv;
    uint64_t ans;
    // printf("RIP @%#lx -> %#lx\n", sgx.rip, *((uint64_t*)sgx.rip));
    rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%lx\n", rv, ans);
    // xed
    return 0;
}
