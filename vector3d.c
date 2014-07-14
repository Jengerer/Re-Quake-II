#include "vector3d.h"
#include "math_common.h"

/*
 * Set all vector components.
 */
void vector3d_set(vector3d_t *vec, float x, float y, float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

/*
 * Clear a vector's components.
 */
void vector3d_clear(vector3d_t *vec)
{
	vector3d_set(vec, 0.0f, 0.0f, 0.0f);
}

/*
 * Return a vector's magnitude.
 */
float vector3d_magnitude(const vector3d_t *vec)
{
	float squared = vector3d_dot_product(vec, vec);
	return sqrtf(squared);
}

/*
 * Calculate unit vector with same direction as vector.
 * Assumes the vector has non-zero magnitude.
 * Returns the vector's magnitude.
 */
float vector3d_normalize(const vector3d_t *vec, vector3d_t *out)
{
	float magnitude = vector3d_magnitude(vec);
	float inverse = 1.0f / magnitude;
	vector3d_scale(vec, inverse, out);
	return magnitude;
}

/*
 * Calculate scalar multiple of a vector.
 */
void vector3d_scale(const vector3d_t *vec, float factor, vector3d_t *out)
{
	out->x = vec->x * factor;
	out->y = vec->y * factor;
	out->z = vec->z * factor;
}

/*
 * Calculate vector in the opposite direction.
 */
void vector3d_negate(const vector3d_t *vec, vector3d_t *out)
{
	out->x = -vec->x;
	out->y = -vec->y;
	out->z = -vec->z;
}

/*
 * Copy a vector to another.
 */
void vector3d_copy(const vector3d_t *vec, vector3d_t *out)
{
	out->x = vec->x;
	out->y = vec->y;
	out->z = vec->z;
}

/*
 * Calculate vector sum (a + b).
 * Assigns out to the value of a + b.
 */
void vector3d_add(const vector3d_t *a, const vector3d_t *b, vector3d_t *out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
}

/*
 * Calculate vector difference.
 * Assigns out to the value of a - b.
 */
void vector3d_subtract(const vector3d_t *a, const vector3d_t *b, vector3d_t *out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
}

/*
 * Calculate and return vector dot product.
 */
float vector3d_dot_product(const vector3d_t *a, const vector3d_t *b)
{
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

/*
 * Calculate and return the vector cross product.
 */
void vector3d_cross_product(const vector3d_t *a, const vector3d_t *b, vector3d_t *out)
{
	// Cache values so we 'out' can be one of the operands.
	float x = (a->y * b->z) - (a->z * b->y);
	float y = (a->z * b->x) - (a->x * b->z);
	out->z = (a->x * b->y) - (a->y * b->x);
	out->y = y;
	out->x = x;
}

/*
 * Convert a vector to a set of Euler angles.
 * Assumes input vector is not the zero vector.
 */
void vector3d_to_angles(const vector3d_t *vector, vector3d_t *out)
{
	float xz_distance;
	float atan_x_z;
	float atan_y_z;

	// Calculate yaw (rotation about Y axis).
	atan_x_z = atan2f(vector->x, vector->z);
	out->y = degrees_to_radians(atan_x_z);

	// Calculate pitch (rotation about X axis).
	xz_distance = sqrtf((vector->x * vector->x) + (vector->z * vector->z));
	atan_y_z = atan2f(vector->y, xz_distance);
	out->x = degrees_to_radians(atan_y_z);
	out->z = 0.0f;
}

/*
 * Add a scalar multiple of some vector to another.
 */
void vector3d_scalar_add(const vector3d_t *base,
	float factor,
	const vector3d_t *scaled,
	vector3d_t *out)
{
	vector3d_scale(scaled, factor, out);
	vector3d_add(base, out, out);
}
