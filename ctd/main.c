#include "ctd.h"
#include "parser.c"
#include "placement.c"

#include <string.h>
#include <stdint.h>
#include <inttypes.h>


int g_cpu_feature_indicator = 0x0;

void dump_hex(uint8_t *buf, int len)
{
    for (int i=0; i < len; i++)
        printf("%02x", *(buf + i));
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <offset> <instruction>\n", argv[0]);
        return 1;
    }
    
    uint64_t offset = parse_hex(argv[1]);
    uint64_t instruction = parse_hex(argv[2]);

    // Create sgx context
    sgx_cpu_context_t sgx;
    memset(&sgx, 0x0, sizeof(sgx)); // set all registers to 0
    // set location of next instruction (Regsiter Instruction Pointer)
    sgx.rip = (uint64_t) put_instruction_on_page(offset, &instruction, 15);
    // sgx.rax = 0xdeadbeefcafebabe;
    // sgx.r8 = 0x0123456789abcdef;
    
    int rv;
    uint64_t ans;
    // printf("RIP @%#lx -> %#lx\n", sgx.rip, *((uint64_t*)sgx.rip));
    rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%lx\n", rv, ans);
    return 0;
}
