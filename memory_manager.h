#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stdlib.h>

// Memory handling functions.
void* memory_allocate(unsigned int size);
void* memory_array_allocate(unsigned int element_size, unsigned int count);
void memory_free(void* buffer);

#endif // _MEMORY_MANAGER_H_