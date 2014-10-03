#pragma once

#include <new.h>

// Static class for managing memory.
class MemoryManager
{

public:

	static void Initialize();
	static void Shutdown();

	// Allocate a buffer of a given size.
	static void *Allocate(unsigned int size);

	// Allocate space for a certain object type.
	// Assumes that object constructor will be called before being deleted.
	template <class Type>
	static bool Allocate(Type **out);

	// Allocate a buffer for an array.
	static void *AllocateArray(unsigned int elementSize, unsigned int count);

	// Free a buffer.
	static void Free(void *buffer);

	// Destroy an object.
	template <class Type>
	static void Destroy(Type *object);

private:

	static int activeAllocations;

};