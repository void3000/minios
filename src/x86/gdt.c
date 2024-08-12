#include <tss.h>

typedef struct
{
	unsigned short	low_limit;
	unsigned short	low_base;
	unsigned char	mid_base;
	unsigned char	access;
	unsigned char	flags;
	unsigned char	high_base;
}__attribute((packed)) gdt_t;

typedef struct
{
	unsigned short	limit;
	unsigned int	base;
}__attribute((packed)) gdt_ptr_t;

gdt_t	gdt[6];
gdt_ptr_t	gdt_ptr;
tss_entry_t system_tss;

void config_tss(void);
void config_gdt(void);
volatile void config_gdt_gate(int nmr,unsigned int base,unsigned int limit,unsigned char access,unsigned char flags);

extern void flush_gdt(void);
extern void flush_tss(void);

volatile void config_gdt_gate(int nmr,unsigned int base,unsigned int limit,unsigned char access,unsigned char flags)
{
	gdt[nmr].low_base	= (base & 0xffff);
	gdt[nmr].mid_base	= ((base >> 16) & 0xff);
	gdt[nmr].high_base	= ((base >> 24) & 0xff);
	gdt[nmr].low_limit	= (limit & 0xffff);
	gdt[nmr].access		= (access);
	gdt[nmr].flags		= ((limit >> 16) & 0xf);
	gdt[nmr].flags		|= (flags & 0xf0);
}

void config_gdt(void)
{
	gdt_ptr.limit	= (sizeof(gdt_t)*6 - 1);
	gdt_ptr.base	= (unsigned int)&gdt;

	memset(&gdt, 0x00, sizeof(gdt_t)*6);

	config_gdt_gate(0x00,0x00,0x00,0x00,0x00); /* null descriptor, selector: 0x00  */
	config_gdt_gate(0x01,0x00,0xffffffff,0x9a,0xcf); /* kernel code, selector: 0x08 */
	config_gdt_gate(0x02,0x00,0xffffffff,0x92,0xcf); /* kernel data, selector: 0x10  */
	config_gdt_gate(0x03,0x00,0xffffffff,0x8a,0xcf); /* user code, selector: 0x18  */
	config_gdt_gate(0x04,0x00,0xffffffff,0x82,0xcf); /* user data, selector: 0x20 */
	config_tss();
	flush_gdt();
	flush_tss();
}

void config_tss(void)
{
	unsigned int base	= ((unsigned int)&system_tss);
	unsigned int limit	= ((unsigned int)( base + sizeof(system_tss)));
	unsigned int system_stack	= 0x00;

	config_gdt_gate(0x05,base,limit,0xe9,0x00);
	memset(&system_tss,0x00,sizeof(tss_entry_t));

	__asm__ volatile ("movl %%esp, %%eax":"=a"(system_stack));
	system_tss.ss0		= 0x10;
	system_tss.esp0		= system_stack;
	system_tss.cs		= (0x08 | 0x03); /* with RPL */
	system_tss.ss	= system_tss.ds = system_tss.es = system_tss.fs = system_tss.gs = (0x10 | 0x03); /* with RPL */
}
