# Overview:

![overview image](images/overview.png)

## Code:

```c
    // Create sgx context
    sgx_cpu_context_t sgx;
    memset(&sgx, 0x0, sizeof(sgx)); // set all registers to 0

    // set location of next instruction (Regsiter Instruction Pointer)
    sgx.rip = (uint64_t) put_instruction_on_page(offset, &instruction, 15);
    sgx.rax = 0xdeadbeefcafebabe;
    sgx.r8 = 0x0123456789abcdef;
```

Data structures:
![structure image](images/structure.png)

- **rip**: Register Instruction Pointer. Pointer to the instruction which is to be executed.
- **rax**: Register Accumulator Extended.
- **r8**: General Purpose Register. Contains extra context

## Return value

### rv

- 0: no access
- 1: read
- 2: write

### ans

absolute adress
