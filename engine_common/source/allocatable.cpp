#include "allocatable.h"
#include "memory_manager.h"

// Scalar new operator.
void *Allocatable::operator new(std::size_t size)
{
	return MemoryManager::Allocate(size);
}

// Scalar delete operator.
void Allocatable::operator delete(void *buffer)
{
	return MemoryManager::Free(buffer);
}

// Array new operator.
void *Allocatable::operator new[](std::size_t size)
{
	return MemoryManager::Allocate(size);
}

// Array delete operator.
void Allocatable::operator delete[](void *buffer)
{
	return MemoryManager::Free(buffer);
}
