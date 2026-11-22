#include "ctd/ctd.h"
#include "xed.h"


/*
Jo zei dat we de registers "slim" moeten vullen.
We moeten nadenken hoe we dat gaan doen, dit is vooral template code.

De functie cleared eerst alle registers,
en vult ze dan met voorgeprogrammeerde waarden
die bepaald werden door een python scriptje.
*/
int fill_registers(sgx_cpu_context_t *sgx){
    if (sgx == NULL) return -1;
    
    memset(&sgx, 0x0, sizeof(sgx)); // set all registers to 0
    
    // now we will set all registers to predefined values
    // e.g. sgx.rax = 0xdeadbeefcafebabe;
    /* <--FILL-PLACEHOLDER--> */
    return 0;
}

/*
Copy the values of the registers from sgx -> to xed_context.
*/
int copy_registers(sgx_cpu_context_t *sgx, xed_context context){
    // Copy the registers
    /* <--COPY-PLACEHOLDER--> */
    return 0;
}