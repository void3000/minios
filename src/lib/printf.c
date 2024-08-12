#include <stdio.h>
#include <stdint.h>
#include <system.h>

int CursorX = 0;
int CursorY = 0;
int attribute =0x07;
unsigned char* video_ptr = (unsigned char*)0xB8000;

int kprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args,fmt);

	for (;*fmt ; fmt++ )
	{
		if (*fmt != '%')
			putc(*fmt);

		else{
			fmt++;
			switch(*fmt)
			{
			case 'c':
				putc(va_arg(args, int));
				break;
			case 's':
				sprintf((char*)va_arg(args, int));
				break;
			case 'd':
			case 'i':
				print_dec(va_arg(args, int));
				break;
			case 'x':
				print_hex(va_arg(args, int));
				break;
			case '%':
				putc('%');
				break;
			default:
				break;
			}
		}
	}

	va_end(args);
	return 0;
}

int format_printf(const char *fmt, va_list args, int VALUE)
{
	return 0;
}

void sprintf(char *s)
{
    __uint8* tmp = (__uint8*)s;
	__uint32 count = 0;
	__uint32 total = 0;

	for (;*tmp != 0; ++total, *tmp++);

    for( count = 0; count < total; putc(s[count]), count++);

	return;
}

int vprintf(const char *fmt, va_list args)
{
	return format_printf(fmt, args, 0);
}

void print_dec(unsigned long value)
{

    signed long tmp = value;
    
    int i = 0;
    int j=0;
    
    char chra[32];
    char chrb[32]; 
    
    if(value == 0)
    {
        putc('0');
        return;
    }
    while (tmp > 0)
    {
        chra[i] = '0' + tmp%10;
        tmp /= 10;
        i++;
    }
    chra[i] = 0;
    chrb[i--] = 0;
    while(i >= 0)
    {
        chrb[i--] = chra[j++];
    }

    sprintf(chrb);
    return;
}

void print_hex(unsigned long hex)
{
	__uint8 tmp		= 0;
	__int8 __zeros	= 1;
	__int32	i		= 0;
	char __hex[16]	= {\
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F',
	};

	sprintf("0x");
	for (i = 28; i >= 0; i -= 4)
	{
		tmp = (hex >> i) & 0x0f;
		putc(__hex[tmp]);
	}
	
	return;
}

void putc(char c)
{
	switch(c)
	{
	case '\n':
	case '\r':
		NewLine:
			CursorX = 0;
			CursorY++;
			if (CursorY >= H_LINES)
				CursorY = 24;
		goto CursorUpdate;
		break;

	case '\t':
		CursorX += 4;
		goto CursorUpdate;
		break;

	default:
		*(video_ptr +(CursorX + CursorY*columns)*2) = c & 0x00FF;
		*(video_ptr +(CursorX  + CursorY*columns)*2 + 1) = (unsigned char)attribute; 
		if ((++CursorX) >= columns)
			goto NewLine;
	}

CursorUpdate:
	update_cursor();
	return;
}

void update_cursor()
{
	unsigned short pos = (CursorY * 80) + CursorX;

	/*	set low byte index to VGA register*/
	outb(DATA_REG, CL_LOW_BYTE);
	outb(INDEX_REG, (unsigned char)(pos & 0xff));

	/*	set high byte index to VGA register*/
	outb(DATA_REG, CL_HIGH_BYTE);
	outb(INDEX_REG, (unsigned char)((pos >> 8) & 0xff));

	return;
}

void clear_screen()
{
	unsigned int i = 0;

	for ( i; i < 80*25; i++ )
	{
		*video_ptr++ = 0;
		*video_ptr++ = 0x07;
	}
	CursorX = 0;
	CursorY = 0;
	update_cursor();
}
