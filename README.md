# Bachelor thesis

## Open vragen

1. Is het goed om XED mee te fuzzen
    ```
    PRO: als we ook coverage guided op XED werken, dan zullen we special cases beter bestuderen.
    Inderdaad, voor special cases zal XED aparte branches hebben, die misschien
    een bug veroorzaken in de CTD.
    CONTRA: kan heel tijd-intensief zijn om dit mee te fuzzen
    ```
2. Moeten we de `CPU_context` mee muteren?
    ```
    PRO: Heel misschien is er ergens een super uitzonderlijke edge case... 
    CONTRA: Kan de fuzzing-focus weghalen van het programma.
    ```

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
XED. The offset is decimal and the instruction is hexadecimal:

```bash
./main 0 0x483c
```

The output includes the CTD result and the XED disassembly, for example:

```text
rv = 0; ans=deadbeefcafebabe
xed = cmp al, 0x48
xed_rv = 0
```

## Fuzz

Fuzzen kan heel gemaklijk met
```bash
make fuzz
```

Achter de schermen gebeurd er dan het volgende:
``` bash
make main #Compile the main-harness with afl (using the afl-gcc compiler)
mkdir input
printf '\x90\xc3' > input/initial_corpus.bin # Start an initial fuzzing corpus
afl-fuzz -T "Daan & Simon" -o output -i input -G 16 ./main # Then run the fuzzer, 
# De flag `-G` bepaalt het maximum aantal bytes.
```

## Clean

```bash
make clean
```
