#pragma once

#include "common_define.h"
#include <cstddef>

// Generic allocatable class for managed heap memory.
class CommonLibrary Allocatable
{

public:

	// Scalar new operator.
	void *operator new(std::size_t size);

	// Scalar delete operator.
	void operator delete(void *buffer);

	// Array new operator.
	void *operator new[](std::size_t size);

	// Array delete operator.
	void operator delete[](void *buffer);

};
