#include "memory_manager.h"
#include <stdio.h>

static memory_manager_t memory_manager;

/* Initialize memory management. */
void memory_manager_initialize(void)
{
	memory_manager.active_allocations = 0;
}

/* Clean up memory manager. */
void memory_manager_destroy(void)
{
	// Dump information.
	fprintf(stderr, "There are %d unfreed memory allocations.\n", memory_manager.active_allocations);
}

/* Allocate base memory chunk. */
void* memory_allocate(unsigned int size)
{
	void *result = malloc(size);
	if (result != NULL) {
		memory_manager.active_allocations += 1;
	}
	return result;
}

/* Allocate array memory chunk. */
void* memory_array_allocate(unsigned int element_size, unsigned int count)
{
	return memory_allocate(element_size * count);
}

/* Free memory chunk. */
void memory_free(void* buffer)
{
	memory_manager.active_allocations -= 1;
	free(buffer);
}
