/*  
 *	Copyright (c) 2013-2014 KM Finger
 *	Licence:	Public domain
 *	Contact:	hobby.sourcecode@gmail.com
 *
 *	Disclaimer:
 *		1. You may use the code in any way you wish.
 *		2. I will not be held responsible for any danger that may occur.
 *		3. Use at your own risk.
 *		4. Give credit if you integrate the code in your project/code.
*/

/*
The 8259 chips can mask (enable and disable) individual irqs.
Zeroing a bit at port 21h or A1h enables the corresponding irq:
--------------------------------------------------------------------------------------------|
Bits at I/O port 21h                                | Bits at I/O port A1h                  |
(1st 8259) 	irq 	                                | 2nd 8259) 	irq                     |
--------------------------------------------------------------------------------------------|
b0 	        irq 0 (timer) 	                        | b0 	irq 8 (realtime clock)          |
b1      	irq 1 (keyboard) 	                    | b1 	irq 9                           |
b2 	        irq 2 (cascade; reserved for 2nd 8259) 	| b2 	irq 10                          |
b3 	        irq 3 (COM2,4) 	                        | b3 	irq 11                          |
b4 	        irq 4 (COM1,3) 	                        | b4 	irq 12 (PS/2 mouse)             |
b5 	        irq 5 (LPT) 	                        | b5 	irq 13 ('386 coprocessor)       |
b6 	        irq 6 (floppy) 	                        | b6 	irq 14 (primary IDE drives)     |
b7 	        irq 7 	                                | b7 	irq 15 (secondary IDE drives)   |
*/

/* All irqs are inactive when 1111 1111
 * All irqs are active when 0000 0000
 */

#include <system.h>

void enable_irq(unsigned int irq);
void disable_irq(unsigned int irq);
void config_irq(void);

void enable_irq(unsigned int irq)
{
    if((irq >=0) & (irq <=15)){
        if (irq < 8){ //0-7. master port
            outb(0x21,inb(0x21) & ~(1 << irq) & 0x00FF); //here we use & and xor to turn on irq
        }else{ //8-15. slaver port
            outb(0xA1, (inb(0xA1) & ~(1 << irq))& 0x00FF);
        }
    }
}

void disable_irq(unsigned int irq)
{
    if((irq >=0) & (irq <=15)){
        if (irq < 8){
            outb(0x21, (inb(0x21)|(1 << irq))&0x00FF); //use or to disable irq
        }else{
            outb(0xA1,(inb(0xA1)| (1 << irq))& 0x00FF);
        }
    }
}

void config_irq(void)
{
	asm volatile ("\
				  movb $0x11, %al;	\
				  out %al, $0x20;	\
				  out %al, $0xA0;	\
				  movb $0x20, %al;	\
				  out %al, $0x21;	\
				  mov $0x28, %al;	\
				  out %al, $0xA1;	\
				  movb $0x04, %al;	\
				  out %al, $0x21;	\
				  mov $0x02, %al;	\
				  out %al, $0xA1;	\
				  movb $0x01, %al;	\
				  out %al, $0x21;	\
				  out %al, $0xA1;	\
				  xorb %al, %al;	\
				  out %al, $0x21;	\
				  out %al, $0xA1;	\
				  ");
}
