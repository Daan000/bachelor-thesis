# Meeting Marco (23-11-2026)

1. Ons CTD Harnass bekijken (zijn er opmerkingen?)
2. Klopt het dat we `rx` en `r8` niet gebruiken?
    ```c
    sgx.rip = (uint64_t)instruction_address;
    sgx.rax = 0xdeadbeefcafebabe;
    sgx.r8 = 0x0123456789abcdef;
    ```
3. Wat moet de fuzzer doen? Enkel `instruction_bytes` en `offset` aanpassen?
    Hoe detecteren we een crash? Met `ReturnValue=-1`?
```c
char *instruction_address = put_instruction_on_page(
    (size_t)offset,
    instruction_bytes,
    sizeof(instruction_bytes)
);
```
4. Wat is de planning voor het komende?

-> lijkt me interessant om een soort kalender in de presentatie te zetten
