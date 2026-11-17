CFLAGS = -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST

ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: ctd_test
# 	objdump -D main

ctd_test: main.c ctd/ctd.c parser.c placement.c ctd/asm.S
	gcc $(CFLAGS) ctd/ctd.c main.c parser.c placement.c ctd/asm.S -o main

clean:
	rm -f ctd_test main
