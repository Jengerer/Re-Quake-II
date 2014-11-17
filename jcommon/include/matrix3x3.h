#pragma once

#include "define.h"

// Class for representing a 3x3 matrix.
class LibraryExport Matrix3x3
{

public:

	// Unary matrix operations.
	void Identity();

	// Assign this matrix the result of a matrix multiplication.
	// This matrix cannot be one of the operands.
	void Product(const Matrix3x3 *a, const Matrix3x3 *b);

private:

	// Helper constant for duplicating code.
	static const int Size = 3;

public:

	float matrixArray[Size][Size];

};