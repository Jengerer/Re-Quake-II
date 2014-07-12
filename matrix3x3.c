#include "matrix3x3.h"

// Constant so we can duplicate code easily for other matrix sizes.
#define MATRIX_SIZE 3

/* Fill the matrix with identity values. */
void matrix3x3_identity(matrix3x3_t *out)
{
	int i;
	int j;

	// Go through each element.
	for (i = 0; i < MATRIX_SIZE; ++i) {
		for (j = 0; j < MATRIX_SIZE; ++j) {
			// Set diagonals to 1, 0 otherwise.
			if (i == j) {
				out->array[i][j] = 1.0f;
			}
			else {
				out->array[i][j] = 0.0f;
			}
		}
	}
}

/*
 * Multiply two matrices together. 
 * The output matrix cannot be one of the operands.
 */
void matrix3x3_multiply(const matrix3x3_t *a, const matrix3x3_t *b, matrix3x3_t *out)
{
	int i;
	int j;
	int k;
	float product;

	// For each element of the output...
	for (i = 0; i < MATRIX_SIZE; ++i) {
		for (j = 0; j < MATRIX_SIZE; ++j) {
			// Element I, J is the dot product of row I of A and column J of B.
			product = 0.0f;
			for (k = 0; k < MATRIX_SIZE; ++k) {
				product += a->array[i][k] * b->array[k][j];
			}
			out->array[i][j] = product;
		}
	}
}
