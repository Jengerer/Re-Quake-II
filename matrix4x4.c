#include "matrix4x4.h"
#include "math_common.h"

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

/* Generate a translation matrix. */
void matrix4x4_translation(const vector3d_t *translation, matrix4x4_t *out)
{
	out->array[0][0] = 1.0f;
	out->array[0][1] = 0.0f;
	out->array[0][2] = 0.0f;
	out->array[0][3] = translation->x;
	out->array[1][0] = 0.0f;
	out->array[1][1] = 1.0f;
	out->array[1][2] = 0.0f;
	out->array[1][3] = translation->y;
	out->array[2][0] = 0.0f;
	out->array[2][1] = 0.0f;
	out->array[2][2] = 1.0f;
	out->array[2][3] = translation->z;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 0.0f;
	out->array[3][3] = 1.0f;
}

/* Generate a rotation matrix around the X axis. */
void matrix4x4_rotation_x(float angle, matrix4x4_t *out)
{
	float angle_radians;
	float cos_angle;
	float sin_angle;

	// Calculate common values.
	angle_radians = degrees_to_radians(angle);
	cos_angle = cosf(angle_radians);
	sin_angle = sinf(angle_radians);

	// Fill in matrix.
	out->array[0][0] = 1.0f;
	out->array[0][1] = 0.0f;
	out->array[0][2] = 0.0f;
	out->array[0][3] = 0.0f;
	out->array[1][0] = 0.0f;
	out->array[1][1] = cos_angle;
	out->array[1][2] = -sin_angle;
	out->array[1][3] = 0.0f;
	out->array[2][0] = 0.0f;
	out->array[2][1] = sin_angle;
	out->array[2][2] = cos_angle;
	out->array[2][3] = 0.0f;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 0.0f;
	out->array[3][3] = 1.0f;
}

/* Generate a perspective projection matrix. */
void matrix4x4_perspective(
	float aspect_ratio,
	float angle_of_view,
	float z_near,
	float z_far,
	matrix4x4_t *out)
{
	float angle_radians;
	float inverse_tan_fov;
	float inverse_far_near_diff;

	// Calculate common values.
	angle_radians = degrees_to_radians(angle_of_view);
	inverse_tan_fov = 1.0f / tanf(angle_radians);
	inverse_far_near_diff = 1.0f / (z_far - z_near);

	// Fill out matrix.
	out->array[0][0] = inverse_tan_fov;
	out->array[0][1] = 0.0f;
	out->array[0][2] = 0.0f;
	out->array[0][3] = 0.0f;
	out->array[1][0] = 0.0f;
	out->array[1][1] = aspect_ratio * inverse_tan_fov;
	out->array[1][2] = 0.0f;
	out->array[1][3] = 0.0f;
	out->array[2][0] = 0.0f;
	out->array[2][1] = 0.0f;
	out->array[2][2] = (z_far + z_near) * inverse_far_near_diff;
	out->array[2][3] = -2.0f * z_far * z_near * inverse_far_near_diff;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 1.0f;
	out->array[3][3] = 0.0f;
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
