>>implemented:
- syscalls 
- timer
- gdt
- idt
- tss
- pic
- printf
- multiboot (grub)

>>not implemented:
- kbrd.c
- cmos.c
- beep.c
- vga.c

>>under construction
-* paging
-* task
- dma,fdc,fat12 (challenge: cannot read more than 1 sector, i.e 512 bytes)
