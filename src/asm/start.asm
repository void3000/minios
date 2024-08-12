global start

start:
	mov esp, _kernel_stack
	jmp entry

	align 4
mboot:
	MULTIBOOT_PAGE_ALIGN	equ 1<<0
	MULTIBOOT_MEMORY_INFO	equ	1<<1
	MULTIBOOT_AOUT_KLUDGE	equ 1<<16
	MULTIBOOT_HEADER_MAGIC	equ 0x1badb002
	MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
	MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	EXTERN code
	EXTERN bss
	EXTERN end

	; GRUB Multiboot header, boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

extern _kernel_main

entry:
	cli

	push eax	; magic value
	push ebx	; pointer to multiboot info struct
	call _kernel_main
	pop ebx
	pop eax

.halt:
	jmp .halt	; halt the system

section .bss

align 4
_kernel_stack:
	resb 0x2800	; 10kb stack which grows downwards below the kernel

