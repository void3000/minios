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

/* Timer ticking
 * 8285 has 3 independent counters controlled by ports 0x40 ~ 0x42.
 * Each counter has 16-bit count register.
 */

#include <stdint.h>
#include <system.h>

volatile unsigned int ticks = 0;

void add_timer(void);
void timer(int Hz);

__volatile__ void timer_handler(void);
__volatile__ void delay(int ms);

__volatile__ void timer_handler(void)
{
	ticks++;
	//__asm__ volatile ("int $0x1c");
}

__volatile__ void delay(int ms)
{
	unsigned int eticks =0;
	unsigned int idle	=0;
	eticks = (ticks + ms);
	while (ticks < eticks){
		idle = 0;
	}
}

/* Initialize the system timer using Hz as a parameter.
 * It is advisable to set frequency to 1000 Hz. 
 * Which is equivent to 1 millisecond.
 */
void timer(int Hz) 
{
	int div	= 1193180 / Hz;
	char byte	= inb(0x21);		// read port PIC 1 = 0x21

	outb(0x43, 0x36); /* bit 6 & 7 choose channel */
	outb(0x40, div & 0xff);
	outb(0x40, div >> 8);

	outb(0x21, byte & 0xfe);
}

asm ("\
	 .global _timer_interrupt;	\
	 .extern _timer_handler;	\
	 \
	 _timer_interrupt:	\
		pushal;	\
		push %ds;	\
		push %es;	\
		push %fs;	\
		push %gs;	\
		movw $0x10, %ax;	\
		movw %ax, %ds;	\
		movw %ax, %es;	\
		movw %ax, %fs;	\
		movw %ax, %gs;	\
		call _timer_handler;	\
		mov $0x20, %al;	\
		out %al, $0x20;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		popal;	\
		iret;	\
	 ");

extern volatile void delay(int);
