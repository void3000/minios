#include <stdint.h>
#include <extern.h>
#include <system.h>
#include <multiboot.h>
#include <filesystem.h>

extern volatile void config_fdc(void);

void kernel_main(unsigned int *mboot_pointer,unsigned int magic_nmr)
{
	PFILE file1;
	char* buff;
	multiboot_header_t *mboot_info;
	mboot_info = (multiboot_header_t*)mboot_pointer;

	__asm__ volatile ("cli");

	config_gdt();
	config_idt();
	config_irq();
	timer(1000);

	__asm__ volatile ("sti");

	config_fdc();

	return;
}
