# Bachelor thesis

## Requirements

- Linux (or WSL)
- GCC
- Intel XED installed in `~/.local/xed-install`
- make

The XED installation must contain `include/xed/xed-interface.h` and
`lib/libxed.so`.

## Build

Build the program with:

```bash
make clean all
```

By default, the `Makefile` uses XED from `~/.local/xed-install`. For another
installation location, pass `XED_DIR`:

```bash
make clean all XED_DIR=/path/to/xed-install
```

Set `CTD_FIX=1` to enable the CTD fix:

```bash
make clean all CTD_FIX=1
```

## Run

`main` runs the CTD decoder and then disassembles the same instruction with
XED:

```bash
./main 0x0 0x483c
```

The output includes the CTD result and the XED disassembly, for example:

```text
rv = 0; ans=deadbeefcafebabe
xed = cmp al, 0x48
xed_rv = 0
```

## Clean

```bash
make clean
```
