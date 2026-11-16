Run in a linux environment (e.g. wsl):

```
make clean all
```

Manual compile:

```
gcc -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST ctd/ctd.c ctd/main.c ctd/asm.S -o ctd/main
```

## Compile xed test script:

```
gcc ai-xed.c -o ai-xed -I/usr/local/include -L/usr/local/lib -lxed && ./ai-xed
```

```
./main 0x0 0x483c
```