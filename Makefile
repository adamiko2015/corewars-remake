CC = gcc
LNK = gcc
NASM = nasm
CFLAGS = -ggdb -Wall -O0 -c
LNKFLAGS = $(CLFAGS)
ASMFLAGS = -f elf64 -g -Wall
SRCs = $(wildcard *.c)
OBJs := $(SRCs:%.c=%.o)

all: main

test:

%.o: %.c
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.asm
		$(NASM) $(ASMFLAGS) $< -o $@

main : $(OBJs)
		$(LNK) $(LNKFLAGS) $^ -o $@

clean:
		rm *.o -f