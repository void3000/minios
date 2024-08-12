;	/*sortware iterrupts*/
[ global _isr_0 ]
[ global _isr_1 ]
[ global _isr_2 ]
[ global _isr_3 ]
[ global _isr_4 ]
[ global _isr_5 ]
[ global _isr_6 ]
[ global _isr_7 ]
[ global _isr_8 ]
[ global _isr_9 ]
[ global _isr_10 ]
[ global _isr_11 ]
[ global _isr_12 ]
[ global _isr_13 ]
[ global _isr_14 ]
[ global _isr_15 ]
[ global _isr_16 ]
[ global _isr_17 ]
[ global _isr_18 ]
[ global _isr_19 ]

[ extern _interrupt_handler ]
[ extern _exception_handler ]

;**********************************************************************************
;Software interrupts
;**********************************************************************************
;	/*division by zero interrupt*/
_isr_0: push dword 0
		push dword 0
		jmp hub_s

;	/*debug interrupt*/
_isr_1:	push dword 0
		push dword 1
		jmp hub_s

;/*Non Maskable Interrupt Exception*/
_isr_2:	push dword 0
		push dword 2
		jmp hub_s

;/*Breakpoint Exception*/
_isr_3:	push dword 0
		push dword 3
		jmp hub_s

;/*Into Detected Overflow Exception*/
_isr_4:	push dword 0
		push dword 4
		jmp hub_s

;/*Out of Bounds Exception*/
_isr_5:	push dword 0
		push dword 5
		jmp hub_s

;/*Invalid Opcode Exception*/
_isr_6:	push dword 0
		push dword 6
		jmp hub_s

;/*No Coprocessor Exception*/
_isr_7:	push dword 0
		push dword 7
		jmp hub_s

;/*Double Fault Exception*/
_isr_8:	push dword 8 ;error code returned
		jmp hub_s
;/*Coprocessor Segment Overrun Exception*/
_isr_9:	push dword 0
		push dword 9
		jmp hub_s

;/*Bad TSS Exception*/
_isr_10: push dword 10 ;error code returned
		 jmp hub_s

;/*Segment Not Present Exception*/
_isr_11: push dword 11 ;error code returned
		 jmp hub_s

;/*Stack Fault Exception*/
_isr_12: push dword 12 ;error code returned
		 jmp hub_s

;/*General Protection Fault Exception*/
_isr_13: push dword 13 ;error code returned
		 jmp hub_s

;/*Page Fault Exception*/
_isr_14: push dword 14 ;error code returned
		 jmp hub_s
;/*Unknown Interrupt Exception*/
_isr_15: push dword 0
		 push dword 15
		 jmp hub_s

;/*Coprocessor Fault Exception*/
_isr_16: push dword 0
		 push dword 16
		 jmp hub_s

;/*Alignment Check Exception (486+)*/
_isr_17: push dword 0
		 push dword 17
		 jmp hub_s

;/*Machine Check Exception (Pentium/586+)*/
_isr_18: push dword 0
	     push dword 18
		 jmp hub_s

;/*Reserved Exception*/
_isr_19: push dword 0
		 push dword 19
		 jmp hub_s

;*********************************************
;central function
;*********************************************
;/*software interrupt*/
hub_s:	pushad	;/*Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI*/
		push ds
		push es
		push fs
		push gs
		mov ax, 0x10	;/*point to kernel data descriptor*/
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		call _exception_handler
		pop gs
		pop fs
		pop es
		pop ds
		popad
		add esp, 8	;/*remove interrupt number and error code from stack*/
iret
