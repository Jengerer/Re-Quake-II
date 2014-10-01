#pragma once

// Class for representing a 3x3 matrix.
class Matrix3x3
{
public:

	// Unary matrix operations.
	void LoadIdentity();

	// Assign this matrix the result of a matrix multiplication.
	// This matrix cannot be one of the operands.
	void MatrixProduct(const Matrix3x3 *a, const Matrix3x3 *b);

private:

	// Helper constant for duplicating code.
	const int MatrixSize = 3;

private:

	float matrix[3][3];

};