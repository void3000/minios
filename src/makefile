LD = ld
CC = gcc
ASM = nasm
START = start.a
OBJ = *.o
ASMFLAGS = -f aout
LDFLAGS = -T linker.ld -o kernel.bin $(START) $(OBJ)  
CCFLAGS = -O -m32 -w -fno-builtin -nostdlib -ffreestanding -Werror -Wall -I ./include -c

all:compile link clean
build: compile link

compile:
	$(CC) $(CCFLAGS) */*.c
	$(CC) $(CCFLAGS) */*/*.c

	$(ASM) $(ASMFLAGS) asm/tables.asm -o tables.o
	$(ASM) $(ASMFLAGS) asm/ivt.asm -o ivt.o
	$(ASM) $(ASMFLAGS) asm/start.asm -o start.a

link:
	$(LD) $(LDFLAGS)

clean:
