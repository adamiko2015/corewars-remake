# This program is for Windows!, you need to account for the linux calling convntion in asm files to change program to linux!

CC = gcc
LNK = gcc
NASM = nasm
CFLAGS = -ggdb -Wall -O0 -c
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