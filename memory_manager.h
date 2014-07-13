#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stdlib.h>

// Structure for memory management.
typedef struct memory_manager
{
	int active_allocations;
} memory_manager_t;

// Manager initialization and destruction.
void memory_manager_initialize(void);
void memory_manager_destroy(void);

// Memory handling functions.
void* memory_allocate(unsigned int size);
void* memory_array_allocate(unsigned int element_size, unsigned int count);
void memory_free(void* buffer);

#endif // _MEMORY_MANAGER_H_