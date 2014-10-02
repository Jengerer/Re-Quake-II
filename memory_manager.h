#pragma once

// Static class for managing memory.
class MemoryManager
{

public:

// Initialization and destruction.

	static void Initialize();
	static void Destroy();

// Allocation management.

	// Allocate a buffer of a given size.
	void *Allocate(unsigned int size);
	// Allocate a buffer for an array.
	void *AllocateArray(unsigned int elementSize, unsigned int count);
	// Free a buffer.
	void Free(void *buffer);

private:

	static int activeAllocations;

};