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

/* Generate a rotation matrix around the Y axis. */
void matrix4x4_rotation_y(float angle, matrix4x4_t *out)
{
	float angle_radians;
	float cos_angle;
	float sin_angle;

	// Calculate common values.
	angle_radians = degrees_to_radians(angle);
	cos_angle = cosf(angle_radians);
	sin_angle = sinf(angle_radians);

	// Fill in matrix.
	out->array[0][0] = cos_angle;
	out->array[0][1] = 0.0f;
	out->array[0][2] = sin_angle;
	out->array[0][3] = 0.0f;
	out->array[1][0] = 0.0f;
	out->array[1][1] = 1.0f;
	out->array[1][2] = 0.0f;
	out->array[1][3] = 0.0f;
	out->array[2][0] = -sin_angle;
	out->array[2][1] = 0.0f;
	out->array[2][2] = cos_angle;
	out->array[2][3] = 0.0f;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 0.0f;
	out->array[3][3] = 1.0f;
}

/* Generate a rotation matrix around the Z axis. */
void matrix4x4_rotation_z(float angle, matrix4x4_t *out)
{
	float angle_radians;
	float cos_angle;
	float sin_angle;

	// Calculate common values.
	angle_radians = degrees_to_radians(angle);
	cos_angle = cosf(angle_radians);
	sin_angle = sinf(angle_radians);

	// Fill in matrix.
	out->array[0][0] = cos_angle;
	out->array[0][1] = -sin_angle;
	out->array[0][2] = 0.0f;
	out->array[0][3] = 0.0f;
	out->array[1][0] = sin_angle;
	out->array[1][1] = cos_angle;
	out->array[1][2] = 0.0f;
	out->array[1][3] = 0.0f;
	out->array[2][0] = 0.0f;
	out->array[2][1] = 0.0f;
	out->array[2][2] = 1.0f;
	out->array[2][3] = 0.0f;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 0.0f;
	out->array[3][3] = 1.0f;
}

/* 
 * Get the transformation matrix for a set of rotation angles.
 * Angles are applied in the order (in object space): Y, X, Z. 
 */
 void matrix4x4_rotation_euler(const vector3d_t *angles, matrix4x4_t *out)
 {
	 matrix4x4_t rotation_x, rotation_y, rotation_z;
	 matrix4x4_t rotation_zx;

	 // Need to apply matrices in reverse order.
	 matrix4x4_rotation_z(angles->z, &rotation_z);
	 matrix4x4_rotation_x(angles->x, &rotation_x);
	 matrix4x4_rotation_y(angles->y, &rotation_y);
	 matrix4x4_multiply(&rotation_x, &rotation_z, &rotation_zx);
	 matrix4x4_multiply(&rotation_y, &rotation_zx, out);
 }

 /* Generate a perspective projection matrix by frustrum parameters. */
 void matrix4x4_frustrum(
	 float left,
	 float right,
	 float top,
	 float bottom,
	 float z_near,
	 float z_far,
	 matrix4x4_t *out)
 {
	float double_z_near;
	float inverse_right_left_diff, inverse_top_bottom_diff, inverse_far_near_diff;

	// Get common values.
	double_z_near = 2.0f * z_near;
	inverse_right_left_diff = 1.0f / (right - left);
	inverse_top_bottom_diff = 1.0f / (top - bottom);
	inverse_far_near_diff = 1.0f / (z_far - z_near);

	// Fill in the matrix.
	out->array[0][0] = double_z_near * inverse_right_left_diff;
	out->array[0][1] = 0.0f;
	out->array[0][2] = (right + left) * inverse_right_left_diff;
	out->array[0][3] = 0.0f;
	out->array[1][0] = 0.0f;
	out->array[1][1] = double_z_near * inverse_top_bottom_diff;
	out->array[1][2] = (top + bottom) * inverse_top_bottom_diff;
	out->array[1][3] = 0.0f;
	out->array[2][0] = 0.0f;
	out->array[2][1] = 0.0f;
	out->array[2][2] = (z_far + z_near) * inverse_far_near_diff;
	out->array[2][3] = -(double_z_near * z_far) * inverse_far_near_diff;
	out->array[3][0] = 0.0f;
	out->array[3][1] = 0.0f;
	out->array[3][2] = 1.0f;
	out->array[3][3] = 0.0f;
 }

/* Generate a perspective projection matrix. */
void matrix4x4_perspective(
	float aspect_ratio,
	float field_of_view,
	float z_near,
	float z_far,
	matrix4x4_t *out)
{
	float fov_radians;
	float x_max, y_max;

	// Calculate common values.
	fov_radians = degrees_to_radians(field_of_view * 0.5f);
	y_max = z_near * tanf(fov_radians);
	x_max = y_max * aspect_ratio;

	// Fill out matrix.
	matrix4x4_frustrum(-x_max, x_max, y_max, -y_max, z_near, z_far, out);
}

/*
 * Get the transpose of a 4x4 matrix.
 */
void matrix4x4_transpose(const matrix4x4_t *mat, matrix4x4_t *out)
{
	int i, j;
	float temp;

	// Copy diagonal.
	for (i = 0; i < MATRIX_SIZE; ++i) {
		out->array[i][i] = mat->array[i][i];
	}

	// Swap elements on opposide sides of the diagonal.
	for (i = 0; i < MATRIX_SIZE; ++i) {
		for (j = i + 1; j < MATRIX_SIZE; ++j) {
			// Keep temporary in case out is the operand.
			temp = mat->array[i][j];
			out->array[i][j] = mat->array[j][i];
			out->array[j][i] = temp;
		}
	}
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

/* Multiply a matrix by a vector. */
void matrix4x4_transform(const matrix4x4_t *mat, const vector4d_t *vec, vector4d_t *out)
{
	out->x = (vec->x * mat->array[0][0]) + (vec->y * mat->array[0][1]) + (vec->z * mat->array[0][2]) + (vec->w * mat->array[0][3]);
	out->y = (vec->x * mat->array[1][0]) + (vec->y * mat->array[1][1]) + (vec->z * mat->array[1][2]) + (vec->w * mat->array[1][3]);
	out->z = (vec->x * mat->array[2][0]) + (vec->y * mat->array[2][1]) + (vec->z * mat->array[2][2]) + (vec->w * mat->array[2][3]);
	out->w = (vec->x * mat->array[3][0]) + (vec->y * mat->array[3][1]) + (vec->z * mat->array[3][2]) + (vec->w * mat->array[3][3]);
}
