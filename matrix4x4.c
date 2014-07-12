#include "matrix4x4.h"

// Constant so we can duplicate code easily for other matrix sizes.
#define MATRIX_SIZE 4

/* Fill the matrix with identity values. */
void matrix4x4_identity(matrix4x4_t *out)
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

/* Fill the last column with a translation vector. */
void matrix4x4_translation(const vector3d_t *translation, matrix4x4_t *out)
{
	out->array[3][0] = translation->x;
	out->array[3][1] = translation->y;
	out->array[3][2] = translation->z;
}

/*
 * Multiply two matrices together. 
 * The output matrix cannot be one of the operands.
 */
void matrix4x4_multiply(const matrix4x4_t *a, const matrix4x4_t *b, matrix4x4_t *out)
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
