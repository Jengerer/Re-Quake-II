#include "memory_manager.h"
#include <stdio.h>
#include <new>

// Allocation count static.
int MemoryManager::activeAllocations = 0;
int MemoryManager::totalMemoryUsage = 0;

// Initialize memory management.
void MemoryManager::Initialize(void)
{
}

// Clean up memory manager.
void MemoryManager::Shutdown(void)
{
	fprintf(stderr, "There are %d unfreed memory allocations.\n", activeAllocations);
}

// Allocate base memory chunk.
void *MemoryManager::Allocate(unsigned int size)
{
	void *result = new (std::nothrow) char[size];
	if (result != nullptr) {
		activeAllocations += 1;
		totalMemoryUsage += size;
	}
	return result;
}

// Free memory chunk.
void MemoryManager::Free(void* buffer)
{
	activeAllocations -= 1;
	delete[] reinterpret_cast<char*>(buffer);
}
