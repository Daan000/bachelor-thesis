#include "ctd.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

int g_cpu_feature_indicator = 0x0;

extern char *ffe_instr;

void dump_hex(uint8_t *buf, int len)
{
    // for (int i=0; i < len; i++)
    //     printf("%02x", *(buf + i));
    // printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <argument>\n", argv[0]);
        return 1;
    }

    const char *input = argv[1];
    char *endptr = NULL;
    errno = 0; // Reset errno before calling strtoull

    // Base 16 parses both "0xdeadbeefcafebabe" and "deadbeefcafebabe"
    unsigned long long parsed = strtoull(input, &endptr, 16);

    // 1. Check if no digits were found at all (e.g., "" or "0x")
    if (endptr == input) {
        fprintf(stderr, "Error: No valid hexadecimal digits found.\n");
        return 1;
    }

    // 2. Check if trailing garbage characters exist (e.g., "0x1234xyz")
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid character '%c' in input.\n", *endptr);
        return 1;
    }

    // 3. Check for overflow (> 0xffffffffffffffff / 16 hex digits)
    if (errno == ERANGE || parsed > UINT64_MAX) {
        fprintf(stderr, "Error: Hex value exceeds 64-bit limit.\n");
        return 1;
    }

    uint64_t value = (uint64_t)parsed;

    // Print using portable inttypes.h macros
    printf("Parsed Hex: 0x%" PRIx64 "\n", value);
    printf("Decimal:    %" PRIu64 "\n", value);


    uint64_t ans;
    int rv;
    sgx_cpu_context_t sgx;

    memset(&sgx, 0x0, sizeof(sgx));
    sgx.rip = (uint64_t) &ffe_instr;
    sgx.rax = parsed;
    sgx.r8 = 0x0123456789abcdef;

    printf("RIP @%#lx -> %#lx\n", sgx.rip, *((uint64_t*)sgx.rip));
    rv = ct_decode(&sgx, &ans);
    printf("\n");
    printf("rv = %d; ans=%lx", rv, ans);
    return 0;
}
