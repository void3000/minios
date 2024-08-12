#include "../include/system.h"

#define p_present_f	(0x01)
#define write_f		(0x02)
#define user_f		(0x04)

#define p_absent_f	(0x00)
#define read_f		(0x00)
#define suuper_v_f	(0x00)

#define page_dir_index(virt_addr) ((unsigned int)((virt_addr >> 22) & 0x3ff))
#define page_tab_index(virt_addr) ((unsigned int)((virt_addr >> 12) & 0x3ff))

void config_paging(void);
void page_fault_handler(x86_reg_t r);
void flush_tbl(unsigned int linear_addr);
void switch_page_directory(unsigned int* new_page_dir);

void switch_page_directory(unsigned int* new_page_dir)
{
	unsigned int *page_dir	= (unsigned int*)new_page_dir;
	__asm__ volatile ("movl %%eax, %%cr3"::"a"(page_dir));
	__asm__ volatile ("\
					  movl %cr0, %eax;	\
					  orl $0x80000000, %eax;	\
					  movl %eax, %cr0;	\
					  ");
	return;
}

void config_paging(void)
{
	unsigned int *page_directory	= (unsigned int*)0x104000;
	unsigned int *page_table		= (unsigned int*)(0x400 + 0x104000);
	unsigned int i =0;
	unsigned int address =0;

	for (i = 0; i < 1024; i++)
	{
		page_directory[i] = 0 | 2; 
	}

	for (i = 0; i < 1024; i++)
	{
		page_table[i] = address | 3; 
		address = address + 4096;
	}

	page_directory[0] = page_table;
	page_directory[0] = page_directory[0] | 3;

	__asm__ volatile ("movl %%eax, %%cr3"::"a"(page_directory));
	__asm__ volatile ("\
					  movl %cr0, %eax;	\
					  orl $0x80000000, %eax;	\
					  movl %eax, %cr0;	\
					  ");
}

/* Invalidating TBL entry. It takes a single linear address.
 * Everytime a frame is accessed, the frame "number" gets
 * cached by the TLB. This technology allows for faster paging
 * as the MMU does not have to check the corresponding frame address
 * in the page table. However, this could be a disadvantage because
 * if the frame is unmarked DIRTY, then the MMU wont know about the
 * update. As a consequence, we have to refresh the TLB.
 */
void flush_tbl(unsigned int linear_addr)
{
	__asm__ volatile ("INVLPG (%%eax)"::"a"(linear_addr));
}

void page_fault_handler(x86_reg_t r)
{
	typedef struct 
	{
		unsigned present : 1;
		unsigned rw	: 1;
		unsigned user	: 1;
		unsigned reserved : 9;
		unsigned page_dir_addr	: 10;
		unsigned page_tab_addr	: 10;
	} __attribute((packed)) page_error_t;

	unsigned int error_code		= 0x00;
	unsigned int fault_addr		= 0x00;
	page_error_t *page_error	= 0x00;

	unsigned char *page_fault_msg[] =
	{
		"Supervisory process tried to read a non-present page entry",
		"Supervisory process tried to read a page and caused a protection fault",
		"Supervisory process tried to write to a non-present page entry",
		"Supervisory process tried to write a page and caused a protection fault",
		"User process tried to read a non-present page entry",
		"User process tried to read a page and caused a protection fault",
		"User process tried to write to a non-present page entry",
		"User process tried to write a page and caused a protection fault",

	};

	asm volatile ("movl %%cr2, %0":"=a"(fault_addr)); /* CR2 register is populated with the linear address that caused the exception */
	
	error_code	= r.error_code;
	page_error = (page_error_t*)&error_code;

	kprintf("%s at linear address %x \n", page_fault_msg[(error_code & 0x07)], fault_addr);
	kprintf("%x \n",page_error->present);
	kprintf("%x \n",page_error->rw);
	kprintf("%x \n",page_error->user);
	kprintf("%x \n",page_error->reserved);
	kprintf("%x \n",page_error->page_dir_addr);
	kprintf("%x \n",page_error->page_tab_addr);

	return;
}
