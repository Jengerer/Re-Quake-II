#include "memory_manager.h"

/* Allocate base memory chunk. */
void* memory_allocate(unsigned int size)
{
	return malloc(size);
}

/* Allocate array memory chunk. */
void* memory_array_allocate(unsigned int element_size, unsigned int count)
{
	return memory_allocate(count * element_size);
}

/* Free memory chunk. */
void memory_free(void* buffer)
{
	free(buffer);
}
