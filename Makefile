CFLAGS = -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST
XED_DIR = $(HOME)/.local/xed-install
XED_CFLAGS = -I$(XED_DIR)/include
XED_LDFLAGS = -L$(XED_DIR)/lib -Wl,-rpath,$(XED_DIR)/lib
XED_LIBS = -lxed

ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: main

main: src/main.c src/xed.c src/xed.h src/ctd/ctd.c src/parser.c src/placement.c src/ctd/asm.S
	gcc $(CFLAGS) $(XED_CFLAGS) src/ctd/ctd.c src/main.c src/xed.c src/parser.c src/placement.c src/ctd/asm.S -o $@ $(XED_LDFLAGS) $(XED_LIBS)

clean:
	rm -f main
