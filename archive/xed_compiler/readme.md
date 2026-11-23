# Compile

Compile an instruction such as `mov rax, [0xdeadbeef]`
to hexadecimal `48 a1 ef be ad de 00 00 00 00`,
(hex: `0x48a1efbeadde00000000`)

One can test this using
```bash
make all
./cli 0 0x48a1efbeadde00000000
```

which returns
```bash
rv = 1; ans=0xdeadbeef
xed = mov rax, qword ptr [0xffffffffdeadbeef]
xed_memop0 = 0xffffffffdeadbeef
```

using the following command:
```bash
export XED_DIR="$HOME/.local/xed-install"
export XED_CFLAGS="-I$XED_DIR/include"
export XED_LDFLAGS="-L$XED_DIR/lib -Wl,-rpath,$XED_DIR/lib"
g++ -O2 xed_compiler.cpp $XED_CFLAGS $XED_LDFLAGS -lxed -o xed_compiler
./xed_compiler
```