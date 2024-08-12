#include <stdint.h>

void strcpy(void* dest, void* src);
extern void strcpy(void* dest, void* src);

/* Compares two strings.
 * If the strings are the same,
 * 1 is returned else 0 is returned
 * when strings are not the same.
 */

void strcpy(void* dest, void* src)
{
    unsigned char* tmp0 = (unsigned char*)dest;
    unsigned char* tmp1 = (unsigned char*)src;
	int t1 = 0;
	for (;*tmp1 != 0; ++t1, *tmp1++);

	for (; (t1 >= 0) ;tmp0[t1] = tmp1[t1], t1-- )
		;

	return;
}
