#pragma once

#include "common_define.h"

// Generic allocatable class for managed heap memory.
class CommonLibrary Allocatable
{

public:

	// Scalar new operator.
	void *operator new(size_t size);

	// Scalar delete operator.
	void operator delete(void *buffer);

	// Array new operator.
	void *operator new[](size_t size);

	// Array delete operator.
	void operator delete[](void *buffer);

};