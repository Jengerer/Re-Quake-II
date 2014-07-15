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

// Calculate the inverse of an affine matrix.

// 3D transformation matrix functions.
void matrix4x4_translation(const vector3d_t *translation, matrix4x4_t *out);
void matrix4x4_rotation_x(float angle, matrix4x4_t *out);
void matrix4x4_rotation_y(float angle, matrix4x4_t *out);
void matrix4x4_rotation_z(float angle, matrix4x4_t *out);
void matrix4x4_rotation_euler(const vector3d_t *angles, matrix4x4_t *out);

// Generate a perspective projection frustrum matrix.
void matrix4x4_frustrum(
	float left,
	float right,
	float top,
	float bottom,
	float z_near,
	float z_far,
	matrix4x4_t *out);

// Generate a perspective projection matrix.
void matrix4x4_perspective(
	float aspect_ratio,
	float field_of_view,
	float z_near,
	float z_far,
	matrix4x4_t *out);

// Unary matrix operations.
void matrix4x4_transpose(const matrix4x4_t *mat, matrix4x4_t *out);

// Binary matrix operations.
void matrix4x4_multiply(const matrix4x4_t *a, const matrix4x4_t *b, matrix4x4_t *out);
void matrix4x4_transform(const matrix4x4_t *mat, const vector4d_t *vec, vector4d_t *out);

#endif // _MATRIX_4X4_H_