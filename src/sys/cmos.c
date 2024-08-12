#include "../include/cmos.h"
#include "../include/system.h"

#define binary(x)	(((x & 0xf0) >> 1) + ((x & 0xf0)>>3) + (x & 0xf))

void  volatile cmos_return_time(__uint8 *hour, __uint8 *min, __uint8 *sec)
{
	__rtc_t rtc;
	/* to avoid inconsistant values */
	loop:
	if (__cmos_updateIn_prog())
		goto loop;

	rtc.sec		= __cmos_return_sec();
	rtc.min		= __cmos_return_min();
	rtc.hour	= __cmos_return_hours();

	do
	{
		rtc.rtc_prv->sec	= rtc.sec;
		rtc.rtc_prv->min	= rtc.min;
		rtc.rtc_prv->hour	= rtc.hour;

		loop2:
		if (__cmos_updateIn_prog())
			goto loop2;

		rtc.sec		= __cmos_return_sec();
		rtc.min		= __cmos_return_min();
		rtc.hour	= __cmos_return_hours();

	}while((rtc.sec != rtc.rtc_prv->sec) || (rtc.min != rtc.rtc_prv->min) || \
			(rtc.hour != rtc.rtc_prv->hour));

	*hour	= binary(rtc.hour);
	*min	= binary(rtc.min);
	*sec	= binary(rtc.sec);
}

void volatile cmos_return_date(__uint8 *month, __uint8 *day, __uint8 *wkday)
{
	__rtc_t rtc;

	/* to avoid inconsistant values */
	loop:
	if (__cmos_updateIn_prog())
		goto loop;

	rtc.wkday	= __cmos_return_weekday();
	rtc.day		= __cmos_return_day();
	rtc.month	= __cmos_return_month();
	rtc.year	= __cmos_return_year();

	do
	{

		rtc.rtc_prv->wkday	= rtc.wkday;
		rtc.rtc_prv->day	= rtc.day;
		rtc.rtc_prv->month	= rtc.month;
		rtc.rtc_prv->year	= rtc.year;

		loop2:
		if (__cmos_updateIn_prog())
			goto loop2;

		rtc.wkday	= __cmos_return_weekday();
		rtc.day		= __cmos_return_day();
		rtc.month	= __cmos_return_month();
		rtc.year	= __cmos_return_year();

	}while((rtc.wkday != rtc.rtc_prv->wkday) || (rtc.day != rtc.rtc_prv->day) || \
			(rtc.month != rtc.rtc_prv->month) || (rtc.year != rtc.rtc_prv->year));
	int i =0;
	*month	= binary(rtc.month);
	*day	= binary(rtc.day) ;
	*wkday	= binary(rtc.wkday);
	return;
}

void cmos_return_floppy_info(__uint8 *a[], __uint8 *b[])
{
	__uint8	tmp	= 0;

	char *drive_type[] = {"Floppy drive not detected", \
					"360kb 5.25inch Floppy drive detected", \
					"1.2mb 5.25inch Floppy drive detected", \
					"720kb 3.5inch Floppy drive detected", \
					"1.44mb 3.5inch Floppy drive detected", \
					"2.88mb 3.5inch Floppy drive detected", 
	};

	tmp = __cmos_read(__cmos_flpy_reg);

	__uint8 __a	= (tmp >> 4) & 0xF; /* drive a */
	__uint8 __b = (tmp & 0x0F); /* drive b */

	*a	= drive_type[__a];
	*b	= drive_type[__b];

	return;
}

char __inline__ volatile __cmos_read(char reg)
{
	outb(__cmos_rw_port, reg);
	return (inb(__cmos_read_indx));
}

char __inline__ volatile __cmos_return_sec()
{
	return __cmos_read(__cmos_sec_reg);
}

char __inline__ volatile __cmos_return_min()
{
	return __cmos_read(__cmos_min_reg);
}

char __inline__ volatile __cmos_return_hours()
{
	return __cmos_read(__cmos_hour_reg);
}

char __inline__ volatile __cmos_return_weekday()
{
	return __cmos_read(__cmos_wkday_reg);
}

char __inline__ volatile __cmos_return_day()
{
	return __cmos_read(__cmos_day_reg);
}

char __inline__ volatile __cmos_return_month()
{
	return __cmos_read(__cmos_month_reg);
}

char __inline__ volatile __cmos_return_year()
{
	return __cmos_read(__cmos_year_reg);
}

char __inline__ volatile __cmos_return_century()
{
	return __cmos_read(__cmos_cent_reg);
}

int __inline__ __cmos_updateIn_prog()
{
	outb(__cmos_rw_port,__cmos_reg_A);
	inb((__cmos_read_indx) & update_flag);
}
