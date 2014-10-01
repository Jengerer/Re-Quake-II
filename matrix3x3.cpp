#include "matrix3x3.h"

// Fill the matrix with identity values.
void Matrix3x3::Identity()
{
	int i, j;
	for (i = 0; i < Size; ++i) {
		for (j = 0; j < Size; ++j) {
			matrixArray[i][j] = (i == j ? 1.0f : 0.0f);
		}
	}
}

// Store result of product of two matrices into this matrix.
// This matrix cannot be one of the operands.
void Matrix3x3::Product(const Matrix3x3 *a, const Matrix3x3 *b)
{
	int i, j, k;

	// For each of our elements...
	for (int i = 0; i < Size; ++i) {
		for (int j = 0; j < Size; ++j) {
			// Element I, J is the dot product of row I of A and column J of B.
			float product = 0.0f;
			for (int k = 0; k < Size; ++k) {
				product += a->matrixArray[i][k] * b->matrixArray[k][j];
			}
			matrixArray[i][j] = product;
		}
	}
}
