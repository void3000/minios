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
#include <idt.h>
#include <system.h>

struct idt_entry_struct
{
	unsigned short		base_low;
	unsigned short		selector;
	unsigned char		always_zero;
	unsigned char		flags;
	unsigned short		base_hi;
}__attribute__((packed));	/*avoid padding*/

struct	idt_ptr_struct
{
	unsigned short		limit;
	unsigned int		base;
}__attribute__((packed));

typedef struct idt_entry_struct	idt_entry_t;
typedef struct idt_ptr_struct	idt_ptr_t;

idt_entry_t	idt_entries[256];
idt_ptr_t	idt_ptr;

void config_idt_gate(unsigned char i, unsigned int base, unsigned short sel, unsigned char flags)
{
   idt_entries[i].base_low	= base & 0xFFFF;
   idt_entries[i].base_hi	= (base >> 16) & 0xFFFF;

   idt_entries[i].selector	= sel;
   idt_entries[i].always_zero = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[i].flags   = flags /* | 0x60 */;
}

void config_idt(void)
{
	idt_ptr.limit = sizeof(idt_entry_t)*256 - 1;
	idt_ptr.base  = (unsigned int)&idt_entries;
	memset(&idt_entries, 0,sizeof(idt_entry_t)*256);
	add_isr();
	add_irq();
	asm volatile ("\
				  lidt (%0);	\
				  "::"r"((unsigned int)&idt_ptr));
}

/* software */
void add_isr(void)
{
	config_idt_gate(0, (unsigned int)isr_0, 0x08, 0x8E);
    config_idt_gate(1, (unsigned int)isr_1, 0x08, 0x8E);
    config_idt_gate(2, (unsigned int)isr_2, 0x08, 0x8E);
    config_idt_gate(3, (unsigned int)isr_3, 0x08, 0x8E);
    config_idt_gate(4, (unsigned int)isr_4, 0x08, 0x8E);
    config_idt_gate(5, (unsigned int)isr_5, 0x08, 0x8E);
    config_idt_gate(6, (unsigned int)isr_6, 0x08, 0x8E);
    config_idt_gate(7, (unsigned int)isr_7, 0x08, 0x8E);

    config_idt_gate(8, (unsigned int)isr_8, 0x08, 0x8E);
    config_idt_gate(9, (unsigned int)isr_9, 0x08, 0x8E);
    config_idt_gate(10, (unsigned int)isr_10, 0x08, 0x8E);
    config_idt_gate(11, (unsigned int)isr_11, 0x08, 0x8E);
    config_idt_gate(12, (unsigned int)isr_12, 0x08, 0x8E);
    config_idt_gate(13, (unsigned int)isr_13, 0x08, 0x8E);
    config_idt_gate(14, (unsigned int)isr_14, 0x08, 0x8E);
    config_idt_gate(15, (unsigned int)isr_15, 0x08, 0x8E);

    config_idt_gate(16, (unsigned int)isr_16, 0x08, 0x8E);
    config_idt_gate(17, (unsigned int)isr_17, 0x08, 0x8E);
    config_idt_gate(18, (unsigned int)isr_18, 0x08, 0x8E);
	config_idt_gate(19, (unsigned int)isr_19, 0x08, 0x8E);

	config_idt_gate(0x1c, (unsigned int)task_interrupt, 0x08, 0x8E);
	config_idt_gate(0x80, (unsigned int)syscall_interrupt, 0x08, 0x8E); /* system call */
}

/* hardware */
void add_irq(void)
{
	config_idt_gate(32,(unsigned int)timer_interrupt, 0x08, 0x8E);
	config_idt_gate(33,(unsigned int)keyboard_interrupt, 0x08, 0x8E);
	config_idt_gate(38,(unsigned int)floppy_disk_controller_interrupt, 0x08, 0x8E);
	/*config_idt_gate(34,(unsigned int)irq_2, 0x08, 0x8E);
	config_idt_gate(35,(unsigned int)irq_3, 0x08, 0x8E);
	config_idt_gate(36,(unsigned int)irq_4, 0x08, 0x8E);
	config_idt_gate(37,(unsigned int)irq_5, 0x08, 0x8E);
	config_idt_gate(39,(unsigned int)irq_7, 0x08, 0x8E);
	config_idt_gate(40,(unsigned int)irq_8, 0x08, 0x8E);
	config_idt_gate(41,(unsigned int)irq_9, 0x08, 0x8E);
	config_idt_gate(42,(unsigned int)irq_10, 0x08, 0x8E);
	config_idt_gate(43,(unsigned int)irq_11, 0x08, 0x8E);
	config_idt_gate(44,(unsigned int)irq_12, 0x08, 0x8E);
	config_idt_gate(45,(unsigned int)irq_13, 0x08, 0x8E);
	config_idt_gate(46,(unsigned int)irq_14, 0x08, 0x8E);
	config_idt_gate(47,(unsigned int)irq_15, 0x08, 0x8E);*/
}
