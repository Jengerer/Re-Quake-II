#ifndef _MATRIX_3X3_H_
#define _MATRIX_3X3_H_

// Structure for a 3x3 matrix.
typedef struct matrix3x3
{
	float array[3][3];
} matrix3x3_t;

// Unary matrix operations.
void matrix3x3_identity(matrix3x3_t *out);

// Binary matrix operations.
void matrix3x3_multiply(const matrix3x3_t *a, const matrix3x3_t *b, matrix3x3_t *out);

#endif // _MATRIX_3X3_H_