#ifndef __system__
#define __system__

#include "stdint.h"

typedef struct
{
    __uint32 gs;
    __uint32 fs;
    __uint32 es;
    __uint32 ds;
    __uint32 edi;
    __uint32 esi;
    __uint32 ebp;
    __uint32 esp;
    __uint32 ebx;
    __uint32 edx;
    __uint32 ecx;
    __uint32 eax;
    __uint32 int_nmr;
    __uint32 error_code;
	__uint32 eip;
	__uint32 cs;
	__uint32 eflags;
	__uint32 usrspace;
	__uint32 ss;
}  /* __attribute__((packed))*/x86_reg_t;

#define ERROR	-1
#define SUCC	1
#define FAIL	0
#define BUSY	2

#define shl(param, times)	(param << times)
#define shr(param, times)	(param >> times)
#define xor(a,b)			( a^b )
#define nible(x, c)		(shr(x,c) & 0x0f)

#define ALIGNMENT		(4)
#define align(size)		(((size) + ALIGNMENT ) & ~(ALIGNMENT - 1))
#define __CALL__(sym)	__asm__ volatile ("call *%0"::"r"(sym))

#define sti(void)		__asm__ volatile ("sti")
#define cli(void)		__asm__ volatile ("cli")

#define outb(port,byte)	__asm__ volatile ("outb %%al, %%dx"::"d"(port), "a"(byte))
#define outw(port,byte)	__asm__ volatile ("outw %%ax, %%dx"::"d"(port), "a"(byte))
#define inb(port)		({char __ret; \
						__asm__ volatile ("inb %w1, %b0":"=a"(__ret):"d"(port));  \
						__ret;})

#endif
