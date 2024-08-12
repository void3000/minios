#ifndef __multiboot__
#define __multiboot__

#include <stdint.h>

#define MULTIBOOT_MAGIC        0x1BADB002
#define MULTIBOOT_EAX_MAGIC    0x2BADB002
#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

typedef struct
{
	__uint32 flags;
	__uint32 memory_lower;
	__uint32 memory_upper;
	__uint32 boot_drive;
	__uint32 cmd_line;
	__uint32 modules_count;
	__uint32 modules_addrs;
	__uint32 num;
	__uint32 size;
	__uint32 addr;
	__uint32 mmap_length;
	__uint32 mmap_addr;
	__uint32 drives_length;
	__uint32 drives_addr;
	__uint32 config_table;
	__uint32 boot_loader_name;
	__uint32 apm_table;
	__uint32 vbe_control_info;
	__uint32 vbe_mode_info;
	__uint32 vbe_interface_seg;
	__uint32 vbe_interface_offset;
	__uint32 vbe_interface_length;

}__attribute__((packed)) multiboot_header_t;

#endif
