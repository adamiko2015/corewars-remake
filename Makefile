# This program is for Windows! You need to account for the linux calling convention in asm files to use program in linux!

CC = gcc
LNK = gcc
NASM = nasm
CFLAGS = -ggdb -Wall -Wextra -O0 -c -std=gnu2x -masm=intel -Wno-unused-parameter
LNKFLAGS = $(CLFAGS)
ASMFLAGS = -f elf64 -g -Wall


C_SRCs = $(wildcard *.c)
C_OBJs := $(C_SRCs:%.c=%.o)

ASM_SRCs = $(wildcard *.asm)
ASM_OBJs := $(ASM_SRCs:%.asm=%.o)

OBJs := $(C_OBJs) $(ASM_OBJs)


all: main

%.o: %.c
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.asm
		$(NASM) $(ASMFLAGS) $< -o $@

main : $(OBJs)
		$(LNK) $(LNKFLAGS) $^ -o $@

clean:
		rm *.o -f