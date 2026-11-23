# Meeting Marco (23-11-2026)

0. Update: c-programma werkt;
1. Ons CTD Harnass bekijken (zijn er opmerkingen?)
2. Wat moet de fuzzer doen? Enkel `instruction_bytes` en `offset` aanpassen?
    Hoe detecteren we een crash? Met `ReturnValue=-1`?
    ```c
    char *instruction_address = put_instruction_on_page(
        (size_t)offset,
        instruction_bytes,
        sizeof(instruction_bytes)
    );
    ```
3. Wat is de planning voor het komende semester?

-> lijkt me interessant om een soort kalender in de presentatie te zetten
