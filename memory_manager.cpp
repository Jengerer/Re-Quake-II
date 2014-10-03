#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>

// Allocation count static.
int MemoryManager::activeAllocations = 0;

// Initialize memory management.
void MemoryManager::Initialize(void)
{
}

// Clean up memory manager.
void MemoryManager::Destroy(void)
{
	fprintf(stderr, "There are %d unfreed memory allocations.\n", activeAllocations);
}

// Allocate base memory chunk.
void *MemoryManager::Allocate(unsigned int size)
{
	void *result = malloc(size);
	if (result != nullptr) {
		++activeAllocations;
	}
	return result;
}

// Allocate array memory chunk.
void *MemoryManager::AllocateArray(unsigned int elementSize, unsigned int count)
{
	return Allocate(elementSize * count);
}

// Free memory chunk.
void MemoryManager::Free(void* buffer)
{
	--activeAllocations;
	free(buffer);
}
