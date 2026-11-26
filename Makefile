CFLAGS = -mavx2 -O2 -masm=intel -g -DCTD_UNIT_TEST
AFL_CC = $(HOME)/AFLplusplus/afl-gcc-fast
# AFL_USE_ASAN=0 # see readme Marco

XED_DIR = $(HOME)/.local/xed-install
XED_CFLAGS = -I$(XED_DIR)/include
XED_LDFLAGS = -L$(XED_DIR)/lib -Wl,-rpath,$(XED_DIR)/lib
XED_LIBS = -lxed


ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: clean main fuzz

xed-compile:
	g++ -O2 xed_compiler.cpp $XED_CFLAGS $XED_LDFLAGS -lxed -o xed_compiler
	./xed_compiler

cli: src/cli.c src/xed.c src/xed.h src/ctd/ctd.c src/parser.c src/placement.c src/ctd/asm.S src/register_fill.c
	gcc $(CFLAGS) $(XED_CFLAGS) src/ctd/ctd.c src/cli.c src/xed.c src/parser.c src/placement.c src/ctd/asm.S src/register_fill.c -o cli $(XED_LDFLAGS) $(XED_LIBS)

fuzz: main
	@bash -c ' \
		trap "echo \"Fuzzing interrupted, running cleanup...\"; python3 scripts/fuzzing_cleanup/main.py" INT; \
		afl-fuzz -T "Daan & Simon" -o output -i input -G 32 -M main ./main \
	'

main: src/main.c src/xed.c src/xed.h src/ctd/ctd.c src/parser.c src/placement.c src/ctd/asm.S src/register_fill.c
	$(AFL_CC) $(CFLAGS) $(XED_CFLAGS) src/ctd/ctd.c src/main.c src/xed.c src/parser.c src/placement.c src/ctd/asm.S src/register_fill.c -o $@ $(XED_LDFLAGS) $(XED_LIBS)

generate_register_fill:
	python3 scripts/register_fill/main.py

clean:
	echo core | sudo tee /proc/sys/kernel/core_pattern
	rm -f main
	rm -rf input
	rm -rf output
	mkdir input
	mkdir output
	printf '\x90\xc3' > input/initial_corpus.bin
