CFLAGS = -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST

ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: ctd_test
	objdump -D main
	./main

ctd_test: main.c ctd.c asm.S
	gcc $(CFLAGS) ctd.c main.c asm.S -o main

clean:
	rm -Rf ctd_test
	clear
