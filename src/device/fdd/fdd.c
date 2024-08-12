#include <stdint.h>
#include <system.h>

typedef struct
{
	__uint32 block;
	__uint8	st0;
	__uint8	cy0;
	__uint8	sector;
	__uint8	track;
	__uint8	head;
	__uint8	drive;

}floppy_t;

/* all floopy disk controller registers
 *
 * FIFO port:
 *	- all data transfer, code result and parameter info use this register
 * MSR register:
 *	- has the "busy" flag
 * DOR port:
 *	- controls the floppy drive motor
 *	- drive selection
 *	- drive reset
 */
enum PRIMARY_ADDRESS{
	\
	status_reg_A	= 0x3f0,
	status_reg_B	= 0x3f1,
	dig_output_reg	= 0x3f2,
	tape_drv_reg	= 0x3f3,
	main_status_reg	= 0x3f4,
	data_rate_reg	= 0x3f4, /* data rate select */
	data_fifo_reg	= 0x3f5,
	dig_input_reg	= 0x3f7,
	config_ctrl_reg	= 0x3f7,
	\
};

/* data output register (DOR) bitflags
 *	- the irq bitflag must be set for dma to function
 *	- the sel_drv bit must also be selected
 *	- the reset bit requires a 4 ms delay
 */
enum{
	\
	sel_drv_0_bit		= 0x00,
	sel_drv_1_bit		= 0x01,
	sel_drv_2_bit		= 0x10,
	sel_drv_3_bit		= 0x11,
	\
	reset_drv_bit		= 0x04,
	\
	set_irq_bit			= 0x08,
	\
	set_motor_drv_0_bit	= 0x10,
	set_motor_drv_1_bit	= 0x20,
	set_motor_drv_2_bit	= 0x40,
	set_motor_drv_3_bit	= 0x80,
	\
};

enum{ /* main status register (MSR) bitflags */
	\
	seek_drv_0_bit	= 0x01,
	seek_drv_1_bit	= 0x02,
	seek_drv_2_bit	= 0x04,
	seek_drv_3_bit	= 0x08,
	\
	msr_busy_bit	= 0x10,
	msr_ndma_bit	= 0x20,
	acknw_byte_bit	= 0x40, /* is set when fifo expects an IN opcode */
	acknw_xchg_bit	= 0x80, /* is set if it's ok to xchg bytes with the fifo i/o port */
	\
};

enum{ /* configuration control register (ccr) bitflags */
	\
	transf_rate_500kb_bit	= 0x00, /* default data transfer rate */
	transf_rate_300kb_bit	= 0x01,
	transf_rate_250kb_bit	= 0x02,
	transf_rate_1mb_bit		= 0x03,
	\
	set_nopr_on_bit		= 0x00,	/* this bit is valid for PS/2 & model 30 */
	set_nopr_off_bit	= 0x01,
	\
};

enum FIFO_COMMANDS{ /* fifo bitflags commands */
	\
	seek_cmd_bit	= 0x0f,
	\
	 /* data transfer commands */
	read_comp_trk	= 0x02, /* readcomplete track */
	write_sect		= 0x05,
	read_sect		= 0x06,
	write_del_sect	= 0x09,
	read_del_sect	= 0x0c,
	format_track	= 0x0d,
	sector_size		= 0x02,
	\
	/* control commands */
	fix_drv_data	= 0x03,
	check_drv_sta	= 0x04,
	fdc_calib_bit	= 0x07,
	seek_int_bit	= 0x08, /* check interrupt status */
	read_sec_id		= 0x0a,
	seek_park_head	= 0x0f,
	double_density	= 0x40,
	skip_del_addr	= 0x20,
	multi_track_op	= 0x80,
	\
};

enum{
	\
	sect_per_trck	= 0x12,
	bytes_per_sect	= 0x200,
	\
	default_drv		= 0x00,
	\
	/* floppy dick sector DTL*/
	sect_dtl_128	= 0x00,
	sect_dtl_256	= 0x01,
	sect_dtl_512	= 0x02,
	sect_dtl_1024	= 0x04,
};

floppy_t floppy;
unsigned char waiting	= false;
unsigned char motorstate = false;

static char results[7]; /* order of results: st0, st1, st2, track, head, sector, sector size */
static char *st0_msg[] =
{
	"Normal termination \nCommand terminated without any errors\n",
	"Abnormal termination \nController started execution of the command, but couldn't terminate it correctly\n",
	"Invalid command \nController couldn't start command execution\n",
	"Abnormal termination by polling \nDrive is not ready\n",
};

#define BYTES_PER_SECTOR	(0x200)
#define SECTOR_SIZE			(BYTES_PER_SECTOR)
#define SECTORS_PER_TRACK	(0x12)
#define FDC_GAP3_LENGTH_3_5	(0x1b)
#define FDC_DATA_LENGTH		(0xff)
#define FDC_DMA_CHANNEL		(0x02)

volatile void config_fdc(void);
volatile void fdc_handler(void);
inline char fdc_read_status(void);
volatile void wait_for_interrupt(void);
void fdc_motor_power(unsigned int state);
int fdc_read_block_fat12(unsigned int addr,unsigned char track, unsigned char head, unsigned char sector);

void fdc_sector_ID(void);
int fdc_calibrate(void);
inline void fdc_send_byte(unsigned char cmd);
unsigned char* read_block_fat12(unsigned int block, unsigned int addr);
void fdc_check_int(unsigned char *st0, unsigned char *cly);
void fdc_convert_lba_to_chs(unsigned int sectorLBA, unsigned char *cylinder, unsigned char *head, unsigned char *sector);

inline void fdc_send_byte(unsigned char cmd)
{
	unsigned count;

	for (count = 0; count < 500; count++){
		if ((fdc_read_status() & 0x80)){
		outb(data_fifo_reg, cmd);
		return;
		}
	}
	kprintf("Command timeout...\n");
}

inline char fdc_read_status(void)
{
	return inb(main_status_reg);
}

int fdc_read_block_fat12(unsigned int addr,unsigned char track, unsigned char head, unsigned char sector)
{
	unsigned count;
	unsigned char st0; unsigned char cly;

	//set up dma for transfer
	if (!dma_read(0x02, addr, 0x200, 0x00)){
		return 0;
	}

	fdc_send_byte(read_sect | double_density | skip_del_addr /*| multi_track_op*/); // read sector command
	fdc_send_byte((head << 2) | floppy.drive );	// drive & head
	fdc_send_byte(track); // cylinder
	fdc_send_byte(head); // head
	fdc_send_byte(sector); // sector
	fdc_send_byte(sector_size); // 512 bytes per sector
	fdc_send_byte(((sector + 1 ) >= 18 ) ? 18 : sector);
	fdc_send_byte(FDC_GAP3_LENGTH_3_5);
	fdc_send_byte(FDC_DATA_LENGTH);

	wait_for_interrupt();

	for (count = 0; count < 8; count++){
		while (!(fdc_read_status() & 0x80)); //ready?
		results[count] = inb(data_fifo_reg);
	}

	/*if ((results[3] != track) | (results[4] != head) | (results[5] != sector)){
		kprintf("%s",st0_msg[(results[0] >> 6)]);
		kprintf("track :%d   head :%d    sector:%d    \n",results[3],results[4],results[5]);
		return (-1);
	}
	*/
	fdc_check_int(&st0,&cly);
	/*
	if ( !((st0 & 0x10) & (cly != track)){
		kprintf("%s",st0_msg[(st0 >> 6)]);
		return (-1);
	}
	*/
	return (1);
}

volatile void fdc_handler(void)
{
	waiting = true;
}

volatile void wait_for_interrupt(void)
{
	while ((waiting == false)){
		dump(0x00,0x00,0x07,"loading...",0,24);
	}
	dump(0x00,0x00,0x07,"          ",0,24);
	waiting = false;
}

void fdc_check_int(unsigned char *st0, unsigned char *cly)
{
	fdc_send_byte(seek_int_bit);
	while (!(fdc_read_status() & 0x80)); //ready?
	*st0 = inb(data_fifo_reg);
	while (!(fdc_read_status() & 0x80));
	*cly = inb(data_fifo_reg);
}

int fdc_calibrate(void)
{
	unsigned char st0; unsigned char cly;

	fdc_motor_power(true);
	fdc_send_byte(fdc_calib_bit);
	fdc_send_byte(floppy.drive);
	wait_for_interrupt();
	fdc_check_int(&st0, &cly);
    if (cly != 0){
        kprintf("%s",st0_msg[(st0 & 0xC0) >> 6]);
        fdc_motor_power(false);
        return -1;
    }
	fdc_motor_power(false);
	return (1);
}

void fdc_sector_ID(void)
{
	unsigned count;

	fdc_send_byte(0xA | double_density);
	fdc_send_byte(0);
	wait_for_interrupt();
	for (count = 0; count < 8; count++){
	while (!(fdc_read_status() & 0x80)); //ready?
	results[count] = inb(data_fifo_reg);
	}
	kprintf("%s",st0_msg[(results[0] >> 6)]);

	kprintf("track :%d   head :%d    sector:%d    \n",results[3],results[4],results[5]);
	return;
}

void fdc_motor_power(unsigned int state)
{
	unsigned i;

	switch(state){
	case false:
		outb(dig_output_reg, 0x0c | floppy.drive); /* turn off motor, Controller enabled, DMA mode */
		delay(300);
		motorstate = 0;
		break;
	case true:
		delay(200);
		outb(dig_output_reg, 0x1c | floppy.drive); /* turn on motor, Controller enabled, DMA mode */
		motorstate = 1;
		break;
	default:
		kprintf("[FDC]: Unknown motor command issued\n");
		break;

	}
}

int fdc_seek(unsigned char head, unsigned char cylinder)
{
	unsigned char st0; unsigned char cly;

	if (!motorstate)
		fdc_motor_power(true);

	fdc_send_byte(seek_park_head);
	fdc_send_byte((head << 2) | floppy.drive);
	fdc_send_byte(cylinder);

	wait_for_interrupt();

	fdc_check_int(&st0, &cly);

	if (cly != cylinder){
		kprintf("%s",st0_msg[(st0 >> 6)]);
		return (-1);
	}
	return (1);
}

void fdc_convert_lba_to_chs(unsigned int sectorLBA, unsigned char *cylinder, unsigned char *head, unsigned char *sector)
{
	*sector = ((sectorLBA % SECTORS_PER_TRACK) + 1);
	*head = ((sectorLBA % (SECTORS_PER_TRACK * 2) ) / SECTORS_PER_TRACK );
	*cylinder = (sectorLBA / (SECTORS_PER_TRACK * 2));
}

unsigned char* read_block_fat12(unsigned int block, unsigned int addr)
{
	if (block > 2880){
		kprintf("Large block issued\n");
		return;
	}
	fdc_convert_lba_to_chs(block, &floppy.track, &floppy.head, &floppy.sector);
	fdc_motor_power(true);
	if (fdc_seek(floppy.head, floppy.track) < 0){
		kprintf("Seek error\n");
		return (0);
	}
	fdc_read_block_fat12(addr, floppy.track, floppy.head, floppy.sector);
	fdc_motor_power(false);
	return (unsigned char*)addr;
}

volatile void config_fdc(void)
{
	floppy.drive	= 0x0000;
	fdc_calibrate();
	outb(config_ctrl_reg, transf_rate_500kb_bit); /* data transfer */
}

asm ("\
	 .global _floppy_disk_controller_interrupt;	\
	 .extern _fdc_handler;	\
	 \
	 _floppy_disk_controller_interrupt:	\
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
		call _fdc_handler;	\
		mov $0x20, %al;	\
		out %al, $0x20;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		popal;	\
		iret;	\
	 ");

extern unsigned char* read_block_fat12(unsigned int, unsigned int);

