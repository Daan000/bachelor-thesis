

als de ctd geen memory access vindt, dan accessed hij 
de geheugenlocatie in de accumulator `rax`.

Daarom interessante tests:

```
./main 0x0 0x483c
```

Returnt de waarde in rax (want het is geen mem access).

Een voorbeeld waarbij het register `r8` opgevraagd wordt, is volgende voorbeeld: ```./main 0x20 0x494f```.

Een voorbeeld voor `rcx`: `./main 0x20 0x49ff`.