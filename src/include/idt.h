/* 	Copyright (c) 2014 KM Finger
;	Licence:	Public domain
;	Contact:	hobby.sourcecode@gmail.com
;
;	Disclaimer:
;		1. You may use the code in any way you wish.
;		2. I will not be held responsible for any danger that may occur.
;		3. Use at your own risk.
;		4. Give credit if you integrate the code in your project/code.
*/
#ifndef __idt__
#define __idt__

#include <stdint.h>
#include <system.h>

/* Interrupt service routine*/
extern void isr_0(void);
extern void isr_1(void);
extern void isr_2(void);
extern void isr_3(void);
extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_19(void);

/*initializes hardware interrupts*/
void add_irq(void);
/*initializes software interrupts*/
void add_isr(void);
/*initializes the idt and interrupts*/
void config_idt(void);

void config_idt_gate(unsigned char i, unsigned int base, unsigned short sel, unsigned char flags);

extern void idt_flush(unsigned int);
extern void syscall_interrupt(void);
extern void timer_interrupt(void);
extern void task_interrupt(void);
extern void keyboard_interrupt(void);
extern void floppy_disk_controller_interrupt(void);

#endif
