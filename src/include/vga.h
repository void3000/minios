#ifndef __vga__
#define __vga__

/*	VGA's I/O ports */

#define VGA_CRTC_INDEX_0		0x3B4	// CRTC Controller Address Register
#define VGA_CRTC_DATA_0			0x3B5	// CRTC Controller Data Register
#define READ_INP_STATUS_REG		0x3BA	// Read -- Input Status #1 Register
#define WRITE_CTRL_REG			0x3BA	// Write -- Feature Control Register
#define	VGA_AC_INDEX			0x3C0	// Attribute Address/Data Register
#define	VGA_AC_WRITE			0x3C0	// Attibute Data/Read Register is special in that it is both address and data-write register.
#define	VGA_AC_READ				0x3C1	// Attibute Data Read Register
#define VGA_MISC_READ			0x3C2	// Read -- Input Status #0 Register
#define	VGA_MISC_WRITE			0x3C2	// Write -- Miscellaneous Output Register
#define VGA_SEQ_INDEX			0x3C4	// Sequence Address Register
#define VGA_SEQ_DATA			0x3C5	// Sequence Data Register
#define	VGA_DAC_READ_INDEX		0x3C7	// Read -- DAC State Register
#define WRITE_DAC_ADDR_REG		0x3C7	// Writes -- DAC Address Read Mode Register
#define	VGA_PALETTE_INDEX		0x3C8	// DAC Address Write Mode Register. Used to set the palette
#define	VGA_PALETTE_DATA		0x3C9	// DAC Data Register. Set palette colors
#define READ_FEATURE_CTRL_REG	0x3CA	// Feature Control Register
#define	VGA_MISC_READ_0			0x3CC	// Read Miscellaneous Output Register
#define VGA_GC_INDEX 			0x3CE	// Graphics Controller Address Register
#define VGA_GC_DATA 			0x3CF	// Graphics Controller Data Register
#define VGA_CRTC_INDEX			0x3D4	// CRTC Controller Address Register
#define VGA_CRTC_DATA			0x3D5	// CRTC Controller Data Register
#define	VGA_INSTAT_READ			0x3DA	// Read -- Input Status #1 Register
#define VGA_INSTAT_WRITE		0x3DA	// Write -- Feature Control Register

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21

void config_vga_mode(int mode);
void vga_update_registers(unsigned char *r);

#endif
