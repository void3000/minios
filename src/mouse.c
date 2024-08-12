#define true	(1)
#define false	(0)

#define PORT_60H	(0x60) // data show up here
#define PORT_64H	(0x64) // status read here

#define PORT_60H_READY_TO_READ	(0x01)
#define PORT_60H_READY_TO_WRITE	(0x02)

#define AUX_DATA	(0x20)

// flags
#define RIGHT_BTN	(0x02)
#define LEFT_BTN	(0x01)
#define MIDDLE_BTN	(0x04)
#define X_SIGN		(0x10)
#define Y_SIGN		(0x20)
#define X_OVERFLOW	(0x40)
#define Y_OVERFLOW	(0x80)

// packet
struct ps2_t{	signed char deltaX;
				signed char deltaY;
				signed char middlebtn;
				unsigned char leftbtn;
				unsigned char rightbtn;
};

signed char mouse_packet[3]	= {0x0000};
signed char mouse_cycle		= 0x0000;

//initiatize
void config_mouse(void);
void HandleMouse(void);

void HandleMouse(void)
{
	struct ps2_t mouse;
	unsigned char var = 0;

	// verify signal it's from mouse
	asm volatile ("\
				  inb $0x64,%%al;	\
				  ":"=a"(var));
	if (var & PORT_60H_READY_TO_READ)
		if (var & AUX_DATA)
			goto check;
	return;

check:
	switch(mouse_cycle){
	case 0:
		asm volatile ("\
				  inb $0x60,%%al;	\
				  ":"=a"(mouse_packet[0]));
		mouse_cycle++;
		break;
	case 1:
		asm volatile ("\
				  inb $0x60,%%al;	\
				  ":"=a"(mouse_packet[1]));
		mouse_cycle++;
		break;
	case 2:
		asm volatile ("\
				  inb $0x60,%%al;	\
				  ":"=a"(mouse_packet[2]));
		mouse.deltaX = mouse_packet[1];
		mouse.deltaY = mouse_packet[2];
		mouse_cycle ^= mouse_cycle;
		goto complete;
		break;
	}
	
complete:
		if ((mouse_packet[0] & X_OVERFLOW) || (mouse_packet[0] & Y_OVERFLOW)){
			return;
		}
		if (mouse_packet[0] & LEFT_BTN){
			mouse.leftbtn = true; 
		}
		if (mouse_packet[0] & RIGHT_BTN){
			mouse.rightbtn = true; 
		}
		if (mouse_packet[0] & MIDDLE_BTN){
			mouse.middlebtn = true; 
		}
}

void aux_input_on(void)
{
	// set compaq status/enable irq12
	asm volatile ("\
				movb $0x20, %al;	\
				outb %al, $0x64;	\
				inb $0x64, %al;	\
				orb $0x02, %al;	\
				xorb $0x20, %al;	\
				xorb %dl, %dl;	\
				xchgb %al, %dl;	\
				movb $0x60, %al;	\
				outb %al, $0x64;	\
				xchgb %dl, %al;	\
				outb %al, $0x60;	\
				  ");

	asm volatile ("\
				  movb $0xA8, %al;	\
				  outb %al, $0x64;	\
				  ");
	// wait for ACK byte
}

void wait_ack(void)
{
	unsigned timeout	= 0x186A0;
	unsigned char var	= 0x0000;
	while (timeout--){
		asm volatile ("inb $0x64, %%al;	\
					  ":"=a"(var));
		if ( var & PORT_60H_READY_TO_READ ){
			return;
		}
	}
	// timeout!
}

void config_mouse(void)
{

}

asm ("\
	 .global _mouse_interrupt;	\
	 .extern _HandleMouse;	\
	 \
	 _mouse_interrupt:	\
		pushal;	\
		push %ds;	\
		push %es;	\
		push %fs;	\
		push %gs;	\
		movw $0x10, %ax;	\
		movw %ax, %ds;	\
		movw %ax, %es;	\
		movw %ax, %fs;	\
		movw %ax, %gs;	\
		call _HandleMouse;	\
		mov $0x20, %al;	\
		out %al, $0x20;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		popal;	\
		iret;	\
	 ");
