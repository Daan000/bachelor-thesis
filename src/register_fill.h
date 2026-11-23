#ifndef REGISTER_FILL_H
#define REGISTER_FILL_H

int fill_registers(sgx_cpu_context_t *sgx, uint64_t instruction_address);
int copy_registers(sgx_cpu_context_t *sgx, xed_context *context);

#endif