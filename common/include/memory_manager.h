#pragma once

#include "common_define.h"
#include <new>

// Allocation structure.
struct Allocation
{
	void *address;
	unsigned int size;
	int allocationIndex;

	// Next index in the list.
	// If this allocation is free, points to next free.
	int nextIndex;
};

// Static class for managing memory.
class CommonLibrary MemoryManager
{

private:

#if defined(_DEBUG)
	// Maximum allocations we can manage.
	static const int MaximumAllocations = 16384;
#endif

public:

	static void Initialize();
	static void Shutdown();

	// Allocate a buffer of a given size.
	static void *Allocate(unsigned int size);

	// Free a buffer.
	static void Free(void *buffer);

#if defined(_DEBUG)
	// Set which allocation to break on.
	static void SetBreakAllocation(int allocationIndex);
#endif

private:

#if defined(_DEBUG)
	static unsigned int activeMemoryUsage;
	static unsigned int peakMemoryUsage;
	static unsigned int totalMemoryUsage;

	// Array/list of allocations.
	static Allocation allocations[MaximumAllocations];

	// Allocation order index.
	static int allocationIndex;

	// Which allocation to break on.
	static int breakAllocation;

	// Index of first used allocation structure.
	static int usedStart;

	// Index of next unused allocation structure.
	static int freeStart;
#endif

};
