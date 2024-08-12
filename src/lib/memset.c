#include <stdint.h>

void *memset(void *dest,char val, size_t count);

extern void *memset(void *dest,char val, size_t count);

void *memset(void *dest,char val, size_t count)
{
    char *tmp = (char*)dest;
	/*for (;count != 0; --count, tmp[count] = val);*/
	do
	{
		--count;
        tmp[count] = val;
	}while (count != 0);
	return dest;
}
