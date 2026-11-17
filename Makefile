CFLAGS = -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST

ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: ctd_test
# 	objdump -D main

ctd_test: src/main.c src/ctd/ctd.c src/parser.c src/placement.c src/ctd/asm.S
	gcc $(CFLAGS) src/ctd/ctd.c src/main.c src/parser.c src/placement.c src/ctd/asm.S -o main

clean:
	rm -f ctd_test main
