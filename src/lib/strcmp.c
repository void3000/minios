#include <stdint.h>

unsigned int strcmp(void *str0, void *str1);
unsigned int strcmpc(void *str0, void* str1,size_t count);

extern unsigned int strcmp(void *str0, void *str1);
extern unsigned int strcmpc(void *str0, void* str1,size_t count);

/* Compares two strings.
 * If the strings are the same,
 * 1 is returned else 0 is returned
 * when strings are not the same.
 */
unsigned int strcmp(void *str0, void* str1)
{
    unsigned char* tmp0 = (unsigned char*)str0;
    unsigned char* tmp1 = (unsigned char*)str1;
	int t0 = 0,t1 = 0;

	for (;*tmp0 != 0; ++t0, *tmp0++);
	for (;*tmp1 != 0; ++t1, *tmp1++);

	if (t0 != t1)
		return 0;

	for (; (t0 != 0) && (t1 != 0), t0--, t1--;)
	{
		if (tmp0[t0] != tmp1[t1])
			return 0;
	}
	return 1;
}

unsigned int strcmpc(void *str0, void* str1,size_t count)
{
    unsigned char* tmp0 = (unsigned char*)str0;
    unsigned char* tmp1 = (unsigned char*)str1;

	for (; count != 0; count--)
	{
		if (tmp0[count] != tmp1[count])
			return 0;
	}
	return 1;
}
