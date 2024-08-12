unsigned char *dump_video_ptr	= (unsigned char*)0xb8000;

void dump_c(unsigned char c, unsigned char attrib, unsigned int x, unsigned int y);
void dump(unsigned char blink, unsigned char bg, unsigned char fbg, unsigned char *s, unsigned int x, unsigned int y); 

void dump(unsigned char blink, unsigned char bg, unsigned char fbg, unsigned char *s, unsigned int x, unsigned int y)
{
	unsigned char attrib = ((blink & 0x80) | bg & 0x70 | fbg & 0xf);
	unsigned int __x = x;
	unsigned int __y = y;
    unsigned char* tmp = (unsigned char*)s;
	unsigned int count = 0;
	unsigned int total = 0;

	for (;*tmp != 0; ++total, *tmp++);

	for (count = 0; count < total; count++)
	{
		dump_c(s[count],attrib,__x,__y);
		__x++;
		if (__x >= 80)
		{
			__y ++;
			__x	 = 0;
		}

	}

	return;
}

void dump_c(unsigned char c,unsigned char attrib, unsigned int x, unsigned int y)
{
	*(dump_video_ptr + (x + y*80)*2)		= (c & 0xff);
	*(dump_video_ptr + (x + y*80)*2 + 1)	= attrib;
}

extern void dump(unsigned char blink, unsigned char bg, unsigned char fbg, unsigned char *s, unsigned int x, unsigned int y);
