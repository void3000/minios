#include <stdint.h>
#include <system.h>

/* DMA commands/registers */
enum{
	\
	DMA_READ_MODE			=	0x04, /* from sector to memory */
	DMA_WRITE_MODE			=	0x08, /* from memory to sector */
	DMA_VERIFY_MODE			=	0x00,
	DMA_INVALID_MODE		=	0x11,
	\
	DMA_AUTO_MODE			=	0x10,
	DMA_DOWN_MODE			=	0x20,
	\
	/* mode of transfer: choose just 1 */
	DMA_TRANSFER_DEMAND_MODE=	0x00,
	DMA_TRANSFER_SINGLE_MODE=	0x80,
	DMA_TRANSFER_BLOCK_MODE	=	0x40,
	DMA_CASCADE_MODE		=	0xC0,
	\
	/* FLIP FLOP RESET COMMANDS */
	RESET_MASTER_FLIPFLOP	=	0xFF,
	RESET_SLAVE_FLIPFLOP	=	0x00,
	\
};

static unsigned char dma_vector[8]={ 0,0,0,0,0,0,0,0,
};

static __uint8 ConvertChannel[8]	= {0,1,2,3,0,1,2,3};
static __uint8 Status_CommandReg[8]	= {0x08, 0x08, 0x08, 0x08, 0xD0, 0xD0, 0xD0, 0xD0};
static __uint8 RequestReg[8]		= {0x09, 0x09,0x09, 0x09, 0xD2,0xD2,0xD2,0xD2};
static __uint8 SingleMaskReg[8]		= {0x0A, 0x0A, 0x0A,0x0A,0xD4, 0xD4, 0xD4, 0xD4}; /* necessary to convert channel.*/
static __uint8 MultiMaskReg[8]		= {0x0F, 0x0F,0x0F, 0x0F,0xDE, 0xDE, 0xDE, 0xDE};
static __uint8 ModeReg[8]			= {0x0B, 0x0B, 0x0B, 0x0B, 0xD6,0xD6,0xD6,0xD6};
static __uint8 FlipFlopReg[8]		= {0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8};
static __uint8 PageAddrReg[8]		= {0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A};
static __uint8 StartAddrReg[8]		= {0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC};
static __uint8 CountReg[8]			= {0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE};

int request_dma_channel(unsigned int channel);
int free_dma_channel(unsigned int channel);
void dma_mask_all_channels(boolean b);
void dma_mask_single_channel(__uint8 DMA_Channel, boolean b);
int dma_read(unsigned char dma_channel, unsigned int addr, size_t length, unsigned int page);
static void __dma_isa_rw(__uint8 DMA_Channel, __uint32 addr, size_t count, __uint32 page_nmr,__uint8 mode);

int request_dma_channel(unsigned int channel)
{
	if (channel > 8)
		return 0;
	if (!dma_vector[channel]){
		dma_vector[channel] = 1;
		return 1;
	}else if (dma_vector[channel]){
		return -1;
	}
}

int free_dma_channel(unsigned int channel)
{
	dma_vector[channel] = 0;
}

void dma_mask_all_channels(boolean b)
{
	if (b == true)
	{
		outb(MultiMaskReg[0], 0xF);
		outb(MultiMaskReg[4], 0xF);
	}else if (b == false)
	{
		outb(MultiMaskReg[0], 0x00);
		outb(MultiMaskReg[4], 0x00);
	}

}

void dma_mask_single_channel(__uint8 DMA_Channel, boolean b)
{
	if (b == true)
		outb(SingleMaskReg[DMA_Channel], 0x04 | ConvertChannel[DMA_Channel]);
	else if (b ==false)
		outb(SingleMaskReg[DMA_Channel], 0x00 | ConvertChannel[DMA_Channel]);
}

/* 
 * - Mode param informs the DMA whether to read or write
 *   from I/O -> MEMORY, vise versa.
 * - Buff param cannot exceed 64 Kb.
 * - Offset must not address beyong 16 Mb.
 * - Note: Some devices do not support AUTO MODE.
 *
 * step 1. Send a byte to Single Mask Channel Register.
 * step 2. Reset the master flip-flop.
 * step 3. Send the mode, Read/Write.
 * step 4. Start Address Register channel. Set to 0x00 (low byte).
 * step 5. Start Address Register channel. Set to 0x10 (high byte).
 * step 6. Reset the master flip-flop again.
 * step 7. Set Count Register channel. Set low byte.
 * step 8. Set Count Register channel. Set high byte.
 * step 9. Set Channel Page Address Register.
 * step 10. Unmask channel.
 */

static void __dma_isa_rw(__uint8 DMA_Channel, __uint32 addr, size_t count, __uint32 page_nmr,__uint8 mode)
{
	__uint8 RESET_FLIPFLOP = 0;

	if (DMA_Channel > 7)
		return;
	RESET_FLIPFLOP =( (DMA_Channel <= 3) ? RESET_MASTER_FLIPFLOP : RESET_SLAVE_FLIPFLOP);

	dma_mask_single_channel(DMA_Channel,true);
	outb(FlipFlopReg[DMA_Channel], RESET_FLIPFLOP );
	outb(ModeReg[DMA_Channel],(unsigned char) (mode | ConvertChannel[DMA_Channel]));
	outb(StartAddrReg[DMA_Channel], (unsigned char)(addr & 0xFF)); /* low byte.*/
	outb(StartAddrReg[DMA_Channel], (unsigned char)((addr & 0xFF00) >> 8)); /* high byte.*/
	outb(FlipFlopReg[DMA_Channel], RESET_MASTER_FLIPFLOP );
	outb(CountReg[DMA_Channel], (unsigned char)(count & 0xFF));
	outb(CountReg[DMA_Channel],(unsigned char)((count & 0xFF00) >> 8));
	outb(PageAddrReg[DMA_Channel], page_nmr); /* No page.*/
	dma_mask_single_channel(DMA_Channel,false);

	return;
}

int dma_read(unsigned char dma_channel, unsigned int addr, size_t length, unsigned int page)
{
	if (dma_channel > 8){
		kprintf("Invalid DMA Channel %d\n", dma_channel);
		return 0;
	}
	if (request_dma_channel(dma_channel) < 0){
		kprintf("DMA channel %d busy\n",dma_channel);
		return 0;
	}
	__dma_isa_rw(dma_channel, addr, length, page, (unsigned char)(DMA_READ_MODE | DMA_TRANSFER_BLOCK_MODE | DMA_AUTO_MODE ));
	free_dma_channel(dma_channel);
	return 1;
}

extern int dma_read(unsigned char dma_channel, unsigned int addr, size_t count, unsigned int page);
