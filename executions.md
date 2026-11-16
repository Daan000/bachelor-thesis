

als de ctd geen memory access vindt, dan accessed hij 
de geheugenlocatie in de accumulator `rax`.

Daarom interessante tests:

```
./main 0x0 0x483c
```

Returnt de waarde in rax (want het is geen mem access).
