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

// Allocate space for a certain object type.
template <class Type>
static bool Allocate(Type **out)
{
	// Allocate space for type.
	Type *object = Allocate(sizeof(Type));
	if (object != nullptr) {
		*out = object;
		return true;
	}
	return false;
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

// Destroy and deallocate an object buffer.
template <class Type>
static void Destroy(Type *object)
{
	// Call destructor and free memory.
	object->~Type();
	Free(object);
}
