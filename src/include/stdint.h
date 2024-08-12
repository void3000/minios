#ifndef __stdint__
#define __stdint__

/*	sizes:
 *		char		: 1 byte
 *		short		: 2 bytes
 *		int			: 4 bytes
 *		long		: 4 bytes
 *		float		: 4 bytes
 *		double		: 8 bytes
 *		long long	: 8 bytes
 */

#define __unsigned		__inline unsigned
#define __signed		__inline signed

/*	standard integer types */
typedef __signed char __int8;
typedef __unsigned char __uint8;
typedef __unsigned char __byte;

typedef __unsigned short __word;
typedef __unsigned int __dword;
typedef __unsigned long long __qword;

typedef __signed short __int16;
typedef __unsigned short __uint16;

typedef __signed int __int32;
typedef __unsigned int __uint32;

typedef __signed long long __int64;
typedef __unsigned long long __uint64;

typedef __unsigned size_t;
typedef __signed boolean;

#define true 1
#define false 0
#define NULL 0

/*	variable ranges*/

/*	__unsigned minimums*/
#define U_INT_MIN		0
#define	U_CHAR_MIN		0
#define	U_SHORT_INT_MIN	0
#define	U_LONG_INT_MIN	0

/*	__signed minimums*/
#define INT_MIN			-2147483648
#define	CHAR_MIN		-127
#define SHORT_INT_MIN	-32768
#define	LONG_INT_MIN	-2147483647

/*	__unsigned maximums*/
#define U_INT_MAX		4294967295
#define	U_CHAR_MAX		255
#define	U_SHORT_INT_MAX	65535
#define	U_LONG_INT_MAX	4294967295

/*	__signed maximums*/
#define INT_MAX			2147483647
#define	CHAR_MAX		127
#define SHORT_INT_MAX	32767
#define	LONG_INT_MAX	2147483647

#endif
