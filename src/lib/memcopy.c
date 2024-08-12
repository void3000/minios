#include <stdint.h>

void *memcopy(void *dest, void *src, size_t count);
extern void *memcopy(void *dest, void *src, size_t count);

/*copies values from a source to destination*/
void *memcopy(void *dest, void *src, size_t count)
{
    unsigned int* tmp_src = (unsigned int*)src;
    unsigned int* tmp_dest = (unsigned int*)dest;
    for (; count !=0; count--, tmp_dest[count] = tmp_src[count]);
    return dest;
}
