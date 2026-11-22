#include "ctd/ctd.h"
#include "xed.h"
#include <string.h>


/*
Jo zei dat we de registers "slim" moeten vullen.
We moeten nadenken hoe we dat gaan doen, dit is vooral template code.

De functie cleared eerst alle registers,
en vult ze dan met voorgeprogrammeerde waarden
die bepaald werden door een python scriptje.
*/
int fill_registers(sgx_cpu_context_t *sgx, uint64_t instruction_address){
    if (sgx == NULL) return -1;
    
    memset(sgx, 0x0, sizeof(*sgx)); // set all registers to 0
    
    // now we will set all registers to predefined values
    // e.g. sgx.rax = 0xdeadbeefcafebabe;
	sgx->rax = 0x0;
	sgx->rcx = 0x1;
	sgx->rdx = 0x2;
	sgx->rbx = 0x3;
	sgx->rsp = 0x4;
	sgx->rbp = 0x5;
	sgx->rsi = 0x6;
	sgx->rdi = 0x7;
	sgx->r8 = 0x8;
	sgx->r9 = 0x9;
	sgx->r10 = 0x10;
	sgx->r11 = 0x11;
	sgx->r12 = 0x12;
	sgx->r13 = 0x13;
	sgx->r14 = 0x14;
	sgx->r15 = 0x15;
	sgx->rflags = 0x16;
	sgx->rip = 0x17;

    sgx->rip = instruction_address;
    return 0;
}

/*
Copy the values of the registers from sgx -> to xed_context.
*/
int copy_registers(sgx_cpu_context_t *sgx, xed_context *context){
    // Copy the registers
	context->rax = sgx->rax;
	context->rcx = sgx->rcx;
	context->rdx = sgx->rdx;
	context->rbx = sgx->rbx;
	context->rsp = sgx->rsp;
	context->rbp = sgx->rbp;
	context->rsi = sgx->rsi;
	context->rdi = sgx->rdi;
	context->r8 = sgx->r8;
	context->r9 = sgx->r9;
	context->r10 = sgx->r10;
	context->r11 = sgx->r11;
	context->r12 = sgx->r12;
	context->r13 = sgx->r13;
	context->r14 = sgx->r14;
	context->r15 = sgx->r15;
	context->rflags = sgx->rflags;
	context->rip = sgx->rip;

    return 0;
}