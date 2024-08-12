/*	Author: KM Finger (c) 2013
	Contribution: Murphy (c) 2013
*/
#ifndef _KEYBOARD_H
#define _KEYBOARD_H
/*include NULL*/
#include "stdint.h"

/*====== Original XT Scan Code Set =======*/

/*	ACPI Scan Codes*/
#define POWER	0
#define	SLEEP	0
#define	WAKE	0

/*	Windows Multimedia Scan Codes*/
#define	NEXT_TRACK	0
#define PRE_TRACK	0
#define STOP		0
#define PLAY_PUASE	0
#define MUTE		0
#define	VOL_UP		0
#define	VOL_DOWN	0
#define MEDIA_SEL	0
#define E_MAIL		0
#define CALC		0
#define	MY_COMP		0
#define WWW_SERACH	0
#define WWW_HOME	0
#define WWW_BACK	0
#define WWW_FORWARD	0
#define WWW_STOP	0
#define WWW_REFRESH	0
#define	WWW_FAVOR	0

/*	Scan Code Set*/

//	ordinary keys
#define ESC			0x1B
#define SPACEBAR	' '
#define TAB			'\t'
#define ENTER		'\n'
#define BACKSPACE	'\b'

#define L_CTRL		0x1D
#define R_CTRL		0x1D
#define L_SHIFT		0x2A
#define R_SHIFT		0x36
#define KP_STAR		0
#define R_ALT		0x38
#define L_ALT		0x38
#define CAPS_LOCK	0x3A

//	external
#define SCROLL_LOCK	0x46	// scroll lock
#define INSERT		0x90
#define DELETE		0x91
#define HOME		0x92
#define END			0x93

// "Meta" keys
#define	META_ALT	0x0200	// Alt is pressed
#define	META_CTRL	0x0400	// Ctrl is pressed
#define	META_SHIFT	0x0800	// Shift is pressed
#define	META_ANY	(META_ALT | META_CTRL | META_SHIFT)
#define	META_CAPS	0x1000	// CapsLock is on
#define	META_NUM	0x2000	// NumLock is on
#define	META_SCRL	0x4000	// ScrollLock is on

//	function keys
#define F1	0x80
#define F2	0x81
#define F3	0x82
#define F4	0x83
#define F5	0x84
#define F6	0x85
#define	F7	0x86
#define	F8	0x87
#define F9	0x88
#define F10	0x89
#define	F11	0x8A
#define F12	0x8B

//	keypad keys
#define NUM_LOCK	0x45		// num lock
#define PG_UP		0x94
#define PG_DOWN		0x95

//	arrows
#define LEFT	0x96
#define UP		0x97
#define DOWN	0x98
#define RIGHT	0x99

/*	keyboard encoder IO*/
#define KYBRD_ENC_INPUT_BUF	0x60			// read
#define KYBRD_ENC_CMD_REG	0x60			// write

/*	onboard keyboard controller IO*/
#define KYBRD_CTRL_STATS_REG	0x64		// read
#define KYBRD_CTRL_CMD_REG		0x64		// write

/*	keyboard encoder commands*/
#define KYBRD_ENC_CMD_SET_LED				0xED
#define	KYBRD_ENC_CMD_ECHO					0xEE
#define KYBRD_ENC_CMD_SCAN_CODE_SET			0xF0
#define KYBRD_ENC_CMD_ID					0xF2
#define KYBRD_ENC_CMD_AUTODELAY				0xF3
#define KYBRD_ENC_CMD_ENABLE				0xF4
#define KYBRD_ENC_CMD_RESETWAIT				0xF5
#define KYBRD_ENC_CMD_RESETSCAN				0xF6
#define KYBRD_ENC_CMD_ALL_AUTO				0xF7
#define KYBRD_ENC_CMD_ALL_MAKEBREAK			0xF8
#define KYBRD_ENC_CMD_ALL_MAKEONLY			0xF9
#define KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO	0xFA
#define KYBRD_ENC_CMD_SINGLE_AUTOREPEAT		0xFB
#define KYBRD_ENC_CMD_SINGLE_MAKEBREAK		0xFC
#define KYBRD_ENC_CMD_SINGLE_BREAKONLY		0xFD
#define KYBRD_ENC_CMD_RESEND				0xFE
#define KYBRD_ENC_CMD_RESET					0xFF

/*	return codes*/
#define KYBRD_ERR_BUF_OVERRUN	0x00
#define KYBRD_ERR_ID_RET		0x83AB
#define KYBRD_ERR_BAT			0xAA
#define KYBRD_ERR_ECHO_RET		0xEE
#define KYBRD_ERR_ACK			0xFA
#define KYBRD_ERR_BAT_FAILED	0xFC
#define KYBRD_ERR_DIAG_FAILED	0xFD
#define KYBRD_ERR_RESEND_CMD	0xFE
#define KYBRD_ERR_KEY			0xFF

/*	onboard keyboard controller commands*/
#define KYBRD_CTRL_CMD_READ				0x20
#define	KYBRD_CTRL_CMD_WRITE			0x60
#define KYBRD_CTRL_CMD_SELF_TEST		0xAA
#define KYBRD_CTRL_CMD_INTERFACE_TEST	0xAB
#define KYBRD_CTRL_CMD_DISABLE			0xAD
#define KYBRD_CTRL_CMD_ENABLE			0xAE
#define KYBRD_CTRL_CMD_READ_IN_PORT		0xC0
#define KYBRD_CTRL_CMD_READ_OUT_PORT	0xD0
#define KYBRD_CTRL_CMD_WRITE_OUT_PORT	0xD1
#define KYBRD_CTRL_CMD_READ_TEST_INPUTS 0xE0
#define KYBRD_CTRL_CMD_SYSTEM_RESET		0xFE
#define KYBRD_CTRL_CMD_MOUSE_DISABLE	0xA7
#define KYBRD_CTRL_CMD_MOUSE_ENABLE		0xAB
#define KYBRD_CTRL_CMD_MOUSE_PORT_TEST  0xA9
#define KYBRD_CTRL_CMD_MOUSE_WRITE		0xD4

/*	onboard keyboard controller status mask*/
#define KYBRD_CTRL_STATS_MASK_OUT_BUF	0x01
#define KYBRD_CTRL_STATS_MASK_IN_BUF	0x02
#define KYBRD_CTRL_STATS_MASK_SYSTEM	0x04
#define KYBRD_CTRL_STATS_MASK_CMD_DATA	0x08
#define KYBRD_CTRL_STATS_MASK_LOCKED	0x10
#define KYBRD_CTRL_STATS_MASK_AUX_BUF	0x20
#define KYBRD_CTRL_STATS_MASK_TIMEOUT	0x40
#define KYBRD_CTRL_STATS_MASK_PARITY	0x80

/*	extra*/
#define KEYPRESS	0x80
#define	PRNT		( 0 + 1 ) //printscreen
#define	PAUSE		( PRNT + 1 )
#define	LWIN		( PAUSE + 1 )
#define	RWIN		( LWIN + 1 )
#define	MENU		( WIN + 1 )

/*	ACSII TABLE*/
/*	New version - 0 shift off and 1 shift on*/
const unsigned char acsii_table[101][2] = { {NULL, NULL}, {ESC, ESC}, {'1','!'}, {'2','@'}, {'3','#'}, {'4','$'}, {'5','%'},
{'6','^'}, {'7','&'}, {'8','*'},{'9','('}, {'0',')'}, {'-','_'},{'=','+'}, {BACKSPACE, BACKSPACE},{TAB, TAB}, {'q','Q'},
{'w','W'}, {'e','E'}, {'r','R'}, {'t','T'}, {'y', 'Y'}, {'u','U'},{'i','I'},{'o','O'},{'p','P'},{'[','{'}, {']','}'},
{ENTER, ENTER},{L_CTRL, L_CTRL}, {'a','A'}, {'s','S'}, {'d','D'}, {'f','F'}, {'g','G'},{'h','H'},{'j','J'},{'k','K'},
{'l','L'},{';',':'},{'\'','"'}, {'`','~'}, {L_SHIFT, L_SHIFT}, {'\\','|'},{'z','Z'},{'x','X'}, {'c','C'}, {'v', 'V'},
{'b','B'}, {'n','N'}, {'m','M'}, {',','<'}, {'.','>'},{'/','?'},{R_SHIFT, R_SHIFT},{PRNT,PRNT}, {L_ALT, L_ALT},
{SPACEBAR,SPACEBAR},{CAPS_LOCK, CAPS_LOCK}, {F1, 0}, {F2, 0},{F3, 0}, {F4, 0}, {F5,0}, {F6,0},{F7,0},{F8,0},
{F9,0},{F10,0},{NUM_LOCK,NUM_LOCK}, {SCROLL_LOCK,SCROLL_LOCK} /*tmp*/,{HOME,HOME},{UP,UP},{PG_UP,PG_UP},{'-','-'},
{LEFT, LEFT},{0,0},{RIGHT,RIGHT},{'+','+'},{END,END},{DOWN, DOWN},{PG_DOWN,PG_DOWN},{INSERT,INSERT},{DELETE, DELETE},
{'/','/'},{ENTER,ENTER},{F11,F11},{F12,F12},{'0',INSERT},{'.',DELETE},{'7',HOME},{'1',END},{'9',PG_UP},{'3',PG_DOWN},
{'4',LEFT},{'6',RIGHT},{'8',UP},{'2',DOWN},{R_ALT,R_ALT},{R_CTRL, R_CTRL},{PAUSE, PAUSE}
};

#endif
