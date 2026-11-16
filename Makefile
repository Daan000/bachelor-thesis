CFLAGS = -mavx2 -O3 -masm=intel -g -DCTD_UNIT_TEST

ifeq ($(CTD_FIX),1)
    CFLAGS += -DCTD_FIX
endif

all: ctd_test
# 	objdump -D ctd/main

ctd_test: ctd/main.c ctd/ctd.c ctd/asm.S
	gcc $(CFLAGS) ctd/ctd.c ctd/main.c ctd/asm.S -o ctd/main

clean:
	rm -Rf ctd_test
	clear
