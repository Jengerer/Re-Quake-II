#pragma once

#include "common_define.h"
#include <new>

// Static class for managing memory.
class CommonLibrary MemoryManager
{

public:

	static void Initialize();
	static void Shutdown();

	// Allocate a buffer of a given size.
	static void *Allocate(unsigned int size);

	// Free a buffer.
	static void Free(void *buffer);

private:

	static int totalMemoryUsage;
	static int activeAllocations;

};
