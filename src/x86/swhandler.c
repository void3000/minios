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

#include <system.h>
#include <extern.h>

void exception_handler(x86_reg_t r);

static void default_exception_handler(x86_reg_t r);


enum INT_TYPE{
	FAULT,
	TRAP,
	ABORT,
};

typedef struct
{
	char			*msg;
	enum INT_TYPE	ERROR_TYPE;
	void			(*func)();
}handle_exceptions_t;

static handle_exceptions_t exception_data[] =
{
	{"Division By Zero Exception",FAULT,&default_exception_handler},
	{"Debug Exception",FAULT,&default_exception_handler},
	{"Non Maskable Interrupt Exception",FAULT,&default_exception_handler},
	{"Breakpoint Exception",TRAP,&default_exception_handler},
	{"Overflow Exceptionion",TRAP,&default_exception_handler},
	{"Bound Range Exceeded Exception",FAULT,&default_exception_handler},
	{"Invalid Opcode Exception",FAULT,&default_exception_handler},
	{"Device Not Available Exception",FAULT,&default_exception_handler},
	{"Double Fault Exception",ABORT,&default_exception_handler},
	{"Coprocessor Segment Overrun Exception",FAULT,&default_exception_handler},
	{"Invalid TSS Exception",FAULT,&default_exception_handler},
	{"Segment Not Present Exception",FAULT,&default_exception_handler},
	{"Stack Segment Fault Exception",FAULT,&default_exception_handler},
	{"General Protection Fault Exception",FAULT,&default_exception_handler},
	{"Page Fault Exception",FAULT,&default_exception_handler},
	{"Unknown Interrupt Exception",FAULT,&default_exception_handler},
	{"x87 Floating-Point Exception",FAULT,&default_exception_handler},
	{"Alignment Check Exception (486+)",FAULT,&default_exception_handler},
	{"Machine Check Exception (Pentium/586+)",ABORT,&default_exception_handler},
	{"SIMD Floating-Point Exception",FAULT,&default_exception_handler},
	{"Reserved",FAULT,&default_exception_handler},
};

static void default_exception_handler(x86_reg_t r)
{

}

void exception_handler(x86_reg_t r)
{
	handle_exceptions_t *handler = 0;
	
	handler = &exception_data[r.int_nmr];
	kprintf("%s (%x) \n", handler->msg, r.int_nmr);
	
	handler->func(r);

	for (;;)
		;
}
