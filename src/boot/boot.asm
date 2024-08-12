;* 	Author: KM FINGER *;
;* 	Created On: 11/11/2013 2:23:53 PM *;
;; 	-	THE BOOTLOADER  IS LOADED AT 0x7c00
;;	-	THE STACK STARTS AT 0x7c00 AND GROWS DOWNWARDS  

bits 16

org  0x7c00

global start

;********************************************************
;			BIOS PARAMETOR BLOCK FAT12
;********************************************************
start: 	
	JMP main							;0x00-0x03
	NOP									;offset		
	OEM					db 'Ubuntu  '	;0x03-0xA, 8 bytes
	BPB_BYTESPERSEC		dw 512			;0xB-0xC
	BPB_SECPERCLUSTER	db 1			;0xD-0xD
	BPB_RESERVEDSEC		dw 1			;0xE-0xF
	BPB_FATs			db 2			;0x10-0x10
	BPB_ROOTDIRENTRIES	dw 224			;0x11-0x12
	BPB_TOTALSECTORS	dw 2880			;0x13-0x14
	BPB_MEDIA			db 240			;0x15-0x15
	BPB_SECPERFAT		dw 9			;0x16-0x17
	BPB_SECPERTRACT		dw 18			;0x18-0x19
	BPB_HSPERCYLINDER	dw 2			;0x1A-0x1B
	BPB_HIDDENSEC		db 0x00			;0x1C-0x1D
	BPB_HUGETOTALSEC	dw 2880			;0x1D-0x20
	BPB_DRIVENUMBER		db 0x00			;0x21-0x21
	BPB_RESERVED		db 0x00			;0x22-0x22
	BPB_BOOTSIGNATURE	db 41			;0x23-0x23
	BPB_VOLUMEID		db 0x01			;0x24-0x27
	BPB_VOLUMENAME		db 'Ubuntu     ';0x28-0x42, 11 bytes
	BPB_FSTYPE			db 'FAT12   '	;0x43-0x4A, 8 bytes

struc fs_fat12
	.drive		resb 1
	.track		resb 1
	.head		resb 1
	.sector		resb 1
	.block		resw 1
endstruc

%define root_dir		(2 * 9 + 1)
%define root_entries	(224*32 / 512)

filename		db "KERNEL  BIN"					;; must be 11 bytes long
msg_error		db "DISK ERROR.", 10, 13, 0
msg_load_file	db "file found", 10, 13, 0
msg_prompt		db "HELLO WORLD.", 10, 13, 0
msg		db "*", 10, 13, 0

main:
	cli
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	xor ax, ax
	mov ss, ax
	mov bp, 0x7c00
	mov sp, bp
	sti
	
	mov byte [fs_fat12.drive], dl	;; get boot drive

	call enable_a20

	cld

	mov si, msg_prompt
	call print

.loop:
	jmp .loop
	hlt

;; @param (es : bx segment:offset of memory location) ax, cx, bx, es
read:
	mov ah, 0x02
	mov al, 0x01
	mov ch, byte [fs_fat12.track]		;; track
	mov cl, byte [fs_fat12.sector]		;; sector
	mov dh, byte [fs_fat12.head]		;; head
	mov dl, byte [fs_fat12.drive]
	int 0x13
	jnc .next
	jmp fault
.next:
	ret

;; @param di is where the data is loacted in memory
;; returns ax, 1 successful, 0 unsuccessful
find_file:
	
	push cx
	mov cx, 11
	mov si, filename
	push di
	res cmpsb
	pop di
	je .found
	pop cx
	add di, 32
	loop find_file
.not_found:
	mov ax, 0
	jmp .done
.found:
	mov ax, 1
.done:
	ret

enable_a20:

	cli
	
	call a20_wait_write
	mov al, 0xad
	out 0x64, al
	
	call a20_wait_write
	mov al, 0xd0
	out 0x60, al
	
	call a20_wait_read
	in al, 0x60	
	push ax
	
	call a20_wait_write
	mov al, 0xd1
	out 0x64, al
	
	call a20_wait_write
	pop ax
	OR al, 00000010b	;; enable a20 line
	out 0x60, al
	
	call a20_wait_write
	mov al, 0xae
	out 0x64, al
	call a20_wait_write
	sti
	ret
	
a20_wait_write:

	in al, 0x64			;; read keyboard controller
	test al, 00000010b
	jnz a20_wait_write
	ret

a20_wait_read:

	in al, 0x64
	test al, 00000001b	;; is buffer full??
	jz a20_wait_read
	ret

print:

	lodsb
	or al, al
	jz .done
	mov ah, 0x0e
	int 0x10
	jmp print
.done:
	ret

reset_drive:

	xor ax, ax
	mov dl, byte[fs_fat12.drive]
	int 0x13
	ret

fault:

	mov si, msg_error
	call print
    mov ah, 0x00
    int 0x16	;; await keypress
    int 0x19	;; warm boot computer
	jmp $

.fill_spaces	times	510 - ($-$$) db 0
.boot_sig		db	0x55, 0xaa
