#include <stdint.h>

unsigned int strlen(void *str);
extern unsigned int strlen(void *str);

/*determines the length of a string*/
unsigned int strlen(void *str)
{
    unsigned char* tmp = (unsigned char*)str;
	unsigned int count = 0;

	for (;*tmp != 0; ++count, *tmp++);
	return count;
}
