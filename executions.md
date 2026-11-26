

als de ctd geen memory access vindt, dan accessed hij 
de geheugenlocatie in de accumulator `rax`.

Daarom interessante tests:

```
./cli 0 0x483c
```

Returnt de waarde in rax (want het is geen mem access).

Een voorbeeld waarbij het register `r8` opgevraagd wordt, is volgende voorbeeld: ```./cli 32 0x494f```.

Een voorbeeld voor `rcx`: `./cli 32 0x49ff`.