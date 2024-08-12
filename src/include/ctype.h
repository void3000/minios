#ifndef __ctype__
#define __ctype__

#define LOWCASE		(0x60)
#define UPPERCASE	(0x40)
#define ALPHAOFFSET	(0x1f)

#define isnum(c)		(((unsigned)(c) >= 48 && (unsigned)(c) <= 57) ? 1 : 0)
#define islowcase(c)	(((unsigned)(c) >= 97 && (unsigned)(c) <= 122) ? 1 : 0)
#define isuppercase(c)	(((unsigned)(c) >= 65 && (unsigned)(c) <= 90) ? 1 : 0)
#define isalpha(c)		((islowercase((unsigned)(c)) || isuppercase((unsigned)(c))) ? 1 : 0)

#define isascii(c)		(((unsigned)(c) <= 0x7f) ? 1 : 0)
#define toascii(c)		((unsigned)(c) & 0x7f))
#define lowercase(c)	((isalpha((unsigned)(c)) == 0 )|| islowercase((unsigned)(c)))? (unsigned)(c) : (unsigned)(c) + ALPHAOFFSET )
#define uppercase(c)	((isalpha((unsigned)(c)) == 0 )|| isuppercase((unsigned)(c)) ? (unsigned)(c) : (unsigned)(c) - ALPHAOFFSET )

#endif
