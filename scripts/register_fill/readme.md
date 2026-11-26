# Register Fill Readme

This program is intended to generate a C-script `register_fill.c`,
which fills the registeres with "interesting" values,
and copy them in the xed_cpu_context.

This output C-script consists of two major parts:

```c
sgx->rax = 0x1;
sgx->rcx = 0x2;
sgx->rdx = 0x3;
sgx->rbx = 0x4;
...
```

and

```c
context->rax = sgx->rax;
context->rcx = sgx->rcx;
context->rdx = sgx->rdx;
...
``` 

## Run the program

```bash
cd src/register_fill && python3 main.py; cd ../..
```
