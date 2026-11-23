#include "ctd/ctd.h"
#include "xed.h"
#include "parser.h"
#include "placement.h"
#include "register_fill.h"

#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Enable AFL++ shared-memory testcase delivery (requires afl-clang-fast / afl-clang-lto)
#ifdef __AFL_FUZZ_TESTCASE_LEN
  __AFL_FUZZ_INIT();
#endif

int g_cpu_feature_indicator = 0x0;

void dump_hex(uint8_t *buf, int len)
{
    for (int i = 0; i < len; i++)
        printf("%02x", *(buf + i));
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 1) 
    {
        fprintf(stderr, "No additional arguments are allowed to %s.\n", argv[0]);
        return 1;
    }

    // --- ONE-TIME INITIALIZATION BEFORE FORKSERVER ---
    // If XED or page allocation can be initialized once, do it here before __AFL_INIT()

#ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
#endif

#ifdef __AFL_FUZZ_TESTCASE_LEN
    unsigned char *fuzz_buf = __AFL_FUZZ_TESTCASE_BUF;
    while (__AFL_LOOP(10000)) 
    {
        int fuzz_len = __AFL_FUZZ_TESTCASE_LEN;
#else
    // Fallback for standalone debugging / reproducing a crash file via stdin
    unsigned char fuzz_buf[4096];
    int fuzz_len = (int)read(STDIN_FILENO, fuzz_buf, sizeof(fuzz_buf));
    if (fuzz_len > 0)
    {
#endif
        input_context input;
        memset(&input, 0, sizeof(input));

        // NOTE: Update read_input() to parse from (fuzz_buf, fuzz_len) instead of stdin!
        int input_rv = read_input_from_buf(&input, fuzz_buf, (size_t)fuzz_len);
        if (input_rv != 0) {
            continue; // Invalid/too-short input; skip cleanly without exiting the loop
        }

        // Set location of next instruction (Register Instruction Pointer)
        // IMPORTANT: Ensure put_instruction_on_page reuses a static/pre-allocated page
        // rather than calling mmap() every iteration without munmap().
        char *instruction_address = put_instruction_on_page(
            (size_t)input.offset,
            input.instruction_bytes,
            sizeof(input.instruction_bytes)
        );
        
        if (instruction_address == NULL) {
            continue; // Offset pushes instruction past page boundary; skip cleanly
        }
        
        // Create sgx context
        sgx_cpu_context_t sgx;
        memset(&sgx, 0, sizeof(sgx));
        fill_registers(&sgx, (uint64_t)instruction_address);
        
        // Copy the registers for later use by XED
        xed_context context;
        memset(&context, 0, sizeof(context));
        copy_registers(&sgx, &context);

        uint64_t ctd_ans = 0;
        int ct_rv = ct_decode(&sgx, &ctd_ans);

#ifdef DEBUG
        printf("\nrv = %d; ans=%#" PRIx64 "\n", ct_rv, ctd_ans);
#else
        (void)ct_rv;
#endif

        xed_answer_t xed_answer;
        memset(&xed_answer, 0, sizeof(xed_answer));

        int xed_rv = disassemble_with_xed(
            input.instruction_bytes,
            sizeof(input.instruction_bytes),
            (uint64_t)instruction_address,
            &context,
            &xed_answer
        );

        if (xed_rv != 0) {
            continue; // Invalid x86 instruction rejected by XED; skip cleanly
        }

#ifdef DEBUG
        print_xed_answer(&xed_answer, context.rax);
#endif

        int comp_rv = compare_xed_ctd(&xed_answer, &ctd_ans, &context);
        if (comp_rv != 0) {
            // Force a crash so AFL++ records the mismatch and saves the testcase
            abort();
        }
    }

    return 0;
}