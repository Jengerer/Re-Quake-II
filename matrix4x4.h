#ifndef _MATRIX_4X4_H_
#define _MATRIX_4X4_H_

#include "vector3d.h"
#include "vector4d.h"

// Structure for a 4x4 matrix.
typedef struct matrix4x4
{
	float array[4][4];
} matrix4x4_t;

// Unary matrix operations.
void matrix4x4_identity(matrix4x4_t *out);
void matrix4x4_translation(const vector3d_t *translation, matrix4x4_t *out);

// Binary matrix operations.
void matrix4x4_multiply(const matrix4x4_t *a, const matrix4x4_t *b, matrix4x4_t *out);

#endif // _MATRIX_4X4_H_