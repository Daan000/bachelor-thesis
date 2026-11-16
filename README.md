Run in a linux environment (e.g. wsl):

```
make clean all
```

Manual compile:

```
gcc -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST ctd/ctd.c ctd/main.c ctd/asm.S -o ctd/main
```

## Compile xed test script:

With XED installed in `xed-workspace/xed-install` (this repo layout):

```
gcc ai-xed.c -o ai-xed \
  -Ixed-workspace/xed-install/include \
  -Lxed-workspace/xed-install/lib -lxed \
  -Wl,-rpath,'$ORIGIN/xed-workspace/xed-install/lib'
./ai-xed
```

If you have XED installed system-wide instead:

```
gcc ai-xed.c -o ai-xed -I/usr/local/include -L/usr/local/lib -lxed && ./ai-xed
```

```
./main 0x0 0x483c
```