#include "memory_manager.h"
#include <intrin.h>
#include <new>
#include <stdio.h>

// Allocation count static.
unsigned int MemoryManager::activeMemoryUsage = 0;
unsigned int MemoryManager::peakMemoryUsage = 0;
unsigned int MemoryManager::totalMemoryUsage = 0;
int MemoryManager::allocationIndex = 0;
int MemoryManager::breakAllocation = -1;
int MemoryManager::usedStart = MaximumAllocations;
int MemoryManager::freeStart = 0;
Allocation MemoryManager::allocations[MaximumAllocations];

// Initialize memory management.
void MemoryManager::Initialize(void)
{
#if defined(_DEBUG)
	// Set up the allocation indices to point to next free.
	// Index of MaximumAllocations indicates no free.
	for (int i = 0; i < MaximumAllocations; ++i) {
		allocations[i].nextIndex = (i + 1);
	}
#endif
}

// Clean up memory manager.
void MemoryManager::Shutdown(void)
{
#if defined(_DEBUG)
	// Print unfreed.
	Allocation *current;
	for (int i = usedStart; i != MaximumAllocations; i = current->nextIndex) {
		current = &allocations[i];
		fprintf(stderr, "Memory leak with allocation #%d of size %u.\n", current->allocationIndex, current->size);
	}
	fprintf(
		stderr,
		"Peak memory usage: %u bytes (%.2f%% of total).\n",
		peakMemoryUsage,
		static_cast<float>(peakMemoryUsage) / static_cast<float>(totalMemoryUsage) * 100.f);
#endif
}

// Allocate base memory chunk.
void *MemoryManager::Allocate(unsigned int size)
{
	void *result = new (std::nothrow) char[size];
#if defined(_DEBUG)
	// Check if break allocation.
	int currentIndex = allocationIndex++;
	if (currentIndex == breakAllocation) {
		__debugbreak();
	}
	if (result != nullptr) {
		// Fill out allocation.
		if (freeStart != MaximumAllocations) {
			// Get the allocation structure.
			Allocation *allocation = &allocations[freeStart];
			allocation->address = result;
			allocation->size = size;
			allocation->allocationIndex = currentIndex;

			// Update next free.
			int oldFreeStart = freeStart;
			freeStart = allocation->nextIndex;

			// Set next node to current allocated head.
			allocation->nextIndex = usedStart;

			// Set as new allocated head.
			usedStart = oldFreeStart;

			// Add this allocation to the allocated list.
			activeMemoryUsage += size;
			if (activeMemoryUsage > peakMemoryUsage) {
				peakMemoryUsage = activeMemoryUsage;
			}
			totalMemoryUsage += size;
		}
		else {
			fprintf(stderr, "Exceeded maximum allocations.\n");
		}
	}
#endif
	return result;
}

// Free memory chunk.
void MemoryManager::Free(void* buffer)
{
	delete[] reinterpret_cast<char*>(buffer);

#if defined(_DEBUG)
	// Find matching allocation.
	int node;
	Allocation *current;
	for (int *reference = &usedStart; (node = *reference) != MaximumAllocations; reference = &current->nextIndex) {
		current = &allocations[node];

		// Does address match?
		if (current->address == buffer) {
			// Update memory metrics.
			activeMemoryUsage -= current->size;

			// Move this node to the free list.
			current->address = nullptr;
			current->size = 0;
			current->allocationIndex = -1;

			// Remove from current list.
			*reference = current->nextIndex;

			// Set as new free head.
			current->nextIndex = freeStart;
			freeStart = node;
			return;
		}
	}

	// Deallocation not in the list.
	fprintf(stderr, "Allocation not in used list: %p\n", buffer);
#endif
}

#if defined(_DEBUG)
// Set which allocation to trigger a debug breakpoint on.
void MemoryManager::SetBreakAllocation(int allocationIndex)
{
	breakAllocation = allocationIndex;
}
#endif
