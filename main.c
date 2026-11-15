#include <string.h>
#include "ctd.h"

int g_cpu_feature_indicator = 0x0;

extern char *ffe_instr;

void dump_hex(uint8_t *buf, int len)
{
    // for (int i=0; i < len; i++)
    //     printf("%02x", *(buf + i));
    // printf("\n");
}

void main(void)
{
    uint64_t ans;
    int rv;
    sgx_cpu_context_t sgx;

    memset(&sgx, 0x0, sizeof(sgx));
    sgx.rip = (uint64_t) &ffe_instr;
    sgx.rax = 0xdeadbeefcafebabe;
    sgx.r8 = 0x0123456789abcdef;

    printf("RIP @%#lx -> %#lx\n", sgx.rip, *((uint64_t*)sgx.rip));
    rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%lx\n", rv, ans);
}
