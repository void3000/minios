; 	x86_gdt.asm
;
; 	Copyright (c) 2014 KM Finger
;	Licence:	Public domain
;	Contact:	hobby.sourcecode@gmail.com
;
;
;	Disclaimer:
;		1. You may use the code in any way you wish.
;		2. I will not be held responsible for any danger that may occur.
;		3. Use at your own risk.
;		4. Give credit if you integrate the code in your project/code.

global _flush_gdt
global _flush_tss
extern _gdt_ptr

_flush_gdt:	; load the gdt

	cli					; disable interrupts
	pusha
	lgdt [_gdt_ptr]		; load gdt

	mov eax, cr0
	or eax,1
	mov cr0, eax	; switch to protected mode

	jmp 0x08 : .flush

.flush:
	mov ax, 0x10	; update segment registers for 32 bit mode
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax
	popa
	sti				; enable interrupts
ret

_flush_tss:
	mov ax, 0x28
	or ax, 0x03	; RPL
	ltr ax
ret
