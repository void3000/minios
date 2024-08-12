#ifndef __cmos__
#define __cmos__

#include "stdint.h"
#include "system.h"

/* cmos ports */
#define __cmos_rw_port		0x70
#define __cmos_read_indx	0x71

#define update_flag	0x80

typedef struct __cmos_rtc_t	/* real-time click */
{
	__uint8	sec;
	__uint8 min;
	__uint8 hour;
	__uint8 wkday;
	__uint8 day;
	__uint8 month;
	__uint8 year;
	__uint8 century;
	struct __cmos_rtc_t	*rtc_prv;
}__rtc_t;

/* cmos registers/indexes */
enum{
	__cmos_sec_reg	= 0x00,
	__cmos_min_reg	= 0x02,
	__cmos_hour_reg	= 0x04,
	__cmos_wkday_reg= 0x06,
	__cmos_day_reg	= 0x07,
	__cmos_month_reg= 0x08,
	__cmos_year_reg	= 0x09,
	__cmos_cent_reg	= 0x32,
	\
	/* status registers */
	__cmos_reg_A	= 0x0A,
	__cmos_reg_B	= 0x0B,
	\
	/* floppy disk drive info */
	__cmos_flpy_reg	= 0x10,

};

int __inline__ __cmos_updateIn_prog();
char  __inline__ volatile __cmos_read(char reg);

void  volatile cmos_return_time(__uint8 *hour, __uint8 *min, __uint8 *sec);
void volatile cmos_return_date(__uint8 *month, __uint8 *day, __uint8 *wkday);

void cmos_return_floppy_info(__uint8 *a[], __uint8 *b[]);

char __inline__ volatile __cmos_return_sec();
char __inline__ volatile __cmos_return_min();
char __inline__ volatile __cmos_return_hours();
char __inline__ volatile __cmos_return_weekday();
char __inline__ volatile __cmos_return_day();
char __inline__ volatile __cmos_return_month();
char __inline__ volatile __cmos_return_year();
char __inline__ volatile __cmos_return_century();

#endif
