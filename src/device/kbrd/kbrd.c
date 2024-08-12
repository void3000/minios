/*  Copyright (c) 2013-2014 KM Finger
 *	Licence:	Public domain
 *	Contact:	hobby.sourcecode@gmail.com
 *
 *	Disclaimer:
 *		1. You may use the code in any way you wish.
 *		2. I will not be held responsible for any danger that may occur.
 *		3. Use at your own risk.
 *		4. Give credit if you integrate the code in your project/code.
 */

#include <stdint.h>
#include <system.h>

#define KEYPRESS	(0x80)
#define L_SHIFT		(0x2A)
#define R_SHIFT		(0x36)

typedef struct
{
	unsigned int shift;
	unsigned int caps;
}keyboard_t;

static const char qwerty[85][2] = {
	{0x00, 0x00}, 
	{0x00, 0x00}, 
	{'1','!'}, {'2', '@'}, {'3','#'},
	{'4','$'}, {'5','%'}, {'6','^'}, {'7','&'}, {'8','*'},
	{'9','('}, {'0',')'}, {'-','_'}, {'=','+'}, 
	{'\b','\b'}, /* backspace */
	{'\t', 0x00}, /* tab */
	{'q','Q'}, {'w','W'}, {'e','E'}, {'r','R'},
	{'t','T'}, {'y','Y'}, {'u','U'}, {'i','I'}, {'o','O'},
	{'p','P'}, {'[','}'}, {']','}'}, {'\n','\n'}, 
	{0x00,0x00}, /* L ctrl */
	{'a','A'}, {'s','S'}, {'d','D'}, {'f','F'}, {'g','G'},
	{'h','H'}, {'j','J'}, {'k','K'}, {'l','L'}, {';',':'},
	{'\'','"'}, {'`','~'},
	{0x00, 0x00}, /* L shift */
	{'\\','|'},
	{'z','Z'}, {'x','X'}, {'c','C'}, {'v','V'},
	{'b','B'}, {'n','N'}, {'m','M'}, {',','<'}, {'.','>'},
	{'/','?'},
	{0x00, 0x00}, /* R shift */
	{0x00,0x00}, /* PrintScr */
	{0x00, 0x00}, /* L alt */
	{' ', ' '}, 
	{0x00, 0x00}, /* Caps lock */
	{0x00,0x00}, /* F1 */
	{0x00,0x00}, /* F2 */
	{0x00,0x00}, /* F3 */
	{0x00,0x00}, /* F4 */
	{0x00,0x00}, /* F5 */
	{0x00,0x00}, /* F6 */
	{0x00,0x00}, /* F7 */
	{0x00,0x00}, /* F8 */
	{0x00,0x00}, /* F9 */
	{0x00,0x00}, /* F10 */
	{0x00,0x00}, /* Num lock*/
	{0x00,0x00}, /* Scroll lock */
	{0x00,'7'}, /* Home */
	{0x00,'8'}, /* Up */
	{0x00,'9'}, /* PageUp */
	{'-','-'},
	{0x00,'4'}, /* Left */
	{0x00,'5'},
	{0x00,'6'}, /* Right */
	{'+','+'},
	{0x00,'1'}, /* End */
	{0x00,'2'}, /* Down */
	{0x00,'3'}, /* PageDown */
	{0x00,'0'}, /* Ins */
	{0x00,'.'}, /* Del */
	{0x00,0x00}, /* F11 */
	{0x00,0x00}, /* F12 */
};

keyboard_t keyboard;

void keyboard_handler(void);

void keyboard_handler(void)
{
	unsigned char scancode;
	unsigned char acsii;

	scancode = inb(0x60);

	if (scancode & KEYPRESS)
	{
		scancode &= ~KEYPRESS;

		kprintf("%c",qwerty[scancode][0]);
	}
}

asm ("\
	 .global _keyboard_interrupt;	\
	 .extern _keyboard_handler;	\
	 \
	 _keyboard_interrupt:	\
		pusha;	\
		push %ds;	\
		push %es;	\
		push %fs;	\
		push %gs;	\
		movw $0x10, %ax;	\
		movw %ax, %ds;	\
		movw %ax, %es;	\
		movw %ax, %fs;	\
		movw %ax, %gs;	\
		call _keyboard_handler;	\
		mov $0x20, %al;	\
		out %al, $0x20;	\
		pop %gs;	\
		pop %fs;	\
		pop %es;	\
		pop %ds;	\
		popa;	\
		iret;	\
	 ");
