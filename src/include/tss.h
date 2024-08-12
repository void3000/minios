#ifndef __tss__
#define __tss__

#include "stdint.h"

typedef struct
{
	__uint32	link;
	__uint32	esp0; /* gets kernel data seg descriptor */
	__uint32	ss0;
	__uint32	esp1;
	__uint32	ss1;
	__uint32	eps2;
	__uint32	ss2;
	__uint32	cr3;
	__uint32	eip;
	__uint32	eflags;
	__uint32	eax;
	__uint32	ecx;
	__uint32	edx;
	__uint32	ebx;
	__uint32	esp;
	__uint32	ebp;
	__uint32	esi;
	__uint32	edi;
	__uint32	es;
	__uint32	cs;
	__uint32	ss;
	__uint32	ds;
	__uint32	fs;
	__uint32	gs;
	__uint32	ldt;
	__uint16	io_bmp;
	__uint16	resv;

}__attribute__((packed)) tss_entry_t;

#endif
