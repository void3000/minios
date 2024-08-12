#include <system.h>
#include <extern.h>

#define MAGIC_HZ	(1193180)

void play(int hz);
void stop(void);
void beep(void);

void play(int hz)
{
	int	 hz_div;
	char tmp;

	hz_div = (MAGIC_HZ / hz);
	outb(0x43, 0xB6);
	outb(0x42, (char)hz_div);	// get lower byte
	outb(0x42, (char)(hz_div >> 8)); // get higher byte

	tmp = inb(0x61);
	if (tmp != (tmp | 3 ) )  /*play sound*/
	{
		outb(0x61, tmp | 3);
	}
}

void stop(void)
{
	char tmp = (inb(0x61) & 0xFC);
	outb(0x61, tmp);
}

void beep(void)
{
	play(501.6);
	delay(100);
	stop();
}
