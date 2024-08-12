/*  Copyright (c) 2013-2014 KM Finger
 *	Licence:	Public domain
 *	Contact:	hobby.sourcecode@gmail.com
 *
 *	Disclaimer:
 *		1. You may use the code in any way you wish.
 *		2. I will not be held responsible for any danger that may occur.
 *		3. Use at your own risk.
 *		4. Give credit if you integrate the code in your project/code.
*/

/* Blue print
 *
 * step 1. Create an array of pointed methods.
 * step 2. Install the system call handler at int 0x80.
 * step 3. System call handler:
 *			- Input number to select method is register eax.
 *			- Pass arguments to the called method by pushing them to stack.
 *			- call the method.
 *			- clear the stack.
 *			- return a value if necessary.
 * Default:
 * eax - the number of the interrupt
 * ebx - ?
 * ecx - length
 * edx - data
 */

#include <syscall.h>

static void* syscalls[__max_syscalls__] = 
{ 
	&kprintf,
	&clear_screen,
	&config_vga_mode,
};

static void syscall_handler(x86_reg_t r)
{
	__volatile__ void *func = syscalls[r.eax];
	__volatile__ int ret = 0;

	__asm__ volatile(" \
		 push %1; \
		 push %2; \
		 push %3; \
		 push %4; \
		 push %5; \
		 call *%6; \
		 pop %%edx; \
		 pop %%ecx; \
		 pop %%ebx; \
		 pop %%edi; \
		 pop %%esi; \
		 "
		:"=a"(ret):"r"(r.esi), "r"(r.edi), "r"(r.ebx),"r"(r.ecx), "r"(r.edx),"r"(func):"memory");

	r.eax = ret;
}

asm ("\
	 .global _syscall_interrupt;	\
	 .extern _syscall_handler;	\
	 \
	 _syscall_interrupt:	\
		push $0x0;	\
		push $0x0;	\
		pusha;	\
		push %ds;	\
		push %es;	\
		push %fs;	\
		push %gs;	\
		movw $0x10, %ax;	\
		movw %ax, %ds;	\
		movw %ax, %es;	\
		movw %ax, %fs;	\
		movw %ax, %gs;	\
		call _syscall_handler;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		add 8, %esp;	\
		popa;	\
		iret;	\
	 ");
