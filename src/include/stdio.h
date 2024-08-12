#ifndef __stdio__
#define __stdio__

#include "stdarg.h"
#include "system.h"

enum{
	\
	H_LINES	= 25,
	columns = 80,
	\
	/* cursor definitions for textmode */
	\
	CL_HIGH_BYTE	= 0x0E,		// Cursor Location High Byte
	CL_LOW_BYTE		= 0x0F,		// Cursor Location Low Byte
	\
	/* CRT registers */
	DATA_REG		= 0x03D4,		// The Data Register is mapped to ports 0x3D4 or 0x3B4.
	INDEX_REG		= 0x03D5,		// The Index Register is mapped to ports 0x3D5 or 0x3B5.
};

int format_printf(const char *fmt, va_list args, int VALUE);
int vprintf(const char *fmt, va_list args);

void putc( char c);
void update_cursor();
void print_dec(unsigned long value);
void print_hex(unsigned long value);

extern void sprintf(char *s);
extern int kprintf(const char *fmt, ...);
extern void add_interrupt_request(int irq_nmr, void (*func)(x86_reg_t r));

#endif
