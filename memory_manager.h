#pragma once

#include <new>

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

	// Free a buffer.
	static void Free(void *buffer);

	// Destroy an object.
	template <class Type>
	static void Destroy(Type *object);

	// Allocate a buffer for an array.
	template <class Type>
	static bool AllocateArray(Type **out, unsigned int count);

	// Destroy array.
	template <class Type>
	static void DestroyArray(Type *buffer);

private:

	static int totalMemoryUsage;
	static int activeAllocations;

};

// Allocate space for a certain object type.
template <class Type>
bool MemoryManager::Allocate(Type **out)
{
	// Allocate space for type.
	Type *object = reinterpret_cast<Type*>(Allocate(sizeof(Type)));
	if (object == nullptr) {
		return false;
	}
	*out = object;
	totalMemoryUsage += sizeof(Type);
	return true;
}

// Allocate array memory chunk.
template <class Type>
bool MemoryManager::AllocateArray(Type **out, unsigned int count)
{
	Type *buffer = new (std::nothrow) Type[count];
	if (buffer == nullptr) {
		return false;
	}
	*out = buffer;
	totalMemoryUsage += sizeof(Type) * count;
	return true;
}

// Destroy array memory chunk.
// Calls destructor 
template <class Type>
void MemoryManager::DestroyArray(Type *buffer)
{
	delete[] buffer;
}

// Destroy and deallocate an object buffer.
template <class Type>
void MemoryManager::Destroy(Type *object)
{
	// Call destructor and free memory.
	object->~Type();
	Free(object);
}