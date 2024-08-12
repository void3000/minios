#ifndef __syscall__
#define __syscall__

#include <system.h>

#define __max_syscalls__	(3)

static void syscall_handler(x86_reg_t r);

extern void clear_screen();
extern void config_vga_mode(int mode);

extern int kprintf(const char *fmt, ...);

#endif
