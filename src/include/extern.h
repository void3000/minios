#ifndef __extern__
#define __extern__

#include "system.h"

extern void timer(int);
extern void config_idt(void);
extern void config_gdt(void);
extern void config_irq(void);

#endif
