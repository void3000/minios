#include "../include/system.h"

typedef struct
{
	unsigned int size;
	struct block_t *next;
}*block_t;

#define heap_end	(0xc00000) // 10 mb
#define heap_start	(0x200000)

/* for simplicity purposes,i will write malloc that dynaically
   alocates blocks in the heap and uses a header.
 */
void* malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *sbrk(size_t size);

size_t new_heap_var = (size_t)heap_start;
size_t old_heap_var = (size_t)heap_start;

void* malloc(size_t size)
{
	size_t block_size	= (align(size + sizeof(block_t)));
	block_t block_info	= sbrk(block_size);

	if (block_info == (void*)-1)
	{
		// error message here
		return (void*)-1;
	}
	block_info->size = (block_size);
	block_info->next = (void*)new_heap_var;

	return (block_info);
}

void free(void* ptr)
{
	block_t block = ptr;
	unsigned int i;
	unsigned char *__ptr = ptr;

	new_heap_var = old_heap_var;

	for (i = 0; i < (block->size + sizeof(block_t)); i ++)
	{
		__ptr[i] = 0;
	}
}

void *sbrk(size_t size)
{
	unsigned int i;
	unsigned char *ptr;

	size_t block_size =(align(size));
	if (block_size > (heap_end - new_heap_var))
	{
		return (void*)-1;
	}
	old_heap_var = new_heap_var;
	new_heap_var = align(new_heap_var + block_size);

	ptr = (unsigned char*)old_heap_var;

	for (i = 0; i < (new_heap_var - old_heap_var); i ++)
	{
		ptr[i] = 0;
	}
	
	return (void*)old_heap_var;
}

extern void* malloc(size_t size);
extern void free(void* ptr);
