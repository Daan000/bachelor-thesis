Run in a linux environment (e.g. wsl):

```
make clean all
```

Manual compile:

```
gcc -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST src/ctd/ctd.c src/main.c src/parser.c src/placement.c src/ctd/asm.S -o main
```

## Compile xed test script:

With XED installed in `xed-workspace/xed-install` (this repo layout):

```
gcc src/ai-xed.c -o ai-xed \
  -Ixed-workspace/xed-install/include \
  -Lxed-workspace/xed-install/lib -lxed \
  -Wl,-rpath,'$ORIGIN/xed-workspace/xed-install/lib'
./ai-xed
```

If you have XED installed system-wide instead:

```
gcc src/ai-xed.c -o ai-xed -I/usr/local/include -L/usr/local/lib -lxed && ./ai-xed
```

```
./main 0x0 0x483c
```