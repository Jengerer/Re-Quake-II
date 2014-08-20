#include "vector4d.h"

/* Set the components of a 4D vector. */
void vector4d_set(vector4d_t *vec, float x, float y, float z, float w)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
	vec->w = w;
}

/* Zero all 4D vector components. */
void vector4d_clear(vector4d_t *vec)
{
	vector4d_set(vec, 0.0f, 0.0f, 0.0f, 0.0f);
}

/* Copy a 4D vector to another. */
void vector4d_copy(const vector4d_t *vec, vector4d_t *out)
{
	out->x = vec->x;
	out->y = vec->y;
	out->z = vec->z;
	out->w = vec->w;
}

/*
 * Convert a 4D (homogeneous) vector to a 3D vector.
 * If the W component is 0.0f, the X, Y, and Z components are unchanged.
 */
void vector4d_to_vector3d(const vector4d_t *vec, vector3d_t *out)
{
	float scale;
	if (vec->w == 0.0f) {
		scale = 1.0f;
	}
	else {
		scale = 1.0f / vec->w;
	}
	out->x = vec->x * scale;
	out->y = vec->y * scale;
	out->z = vec->z * scale;
}

/*
 * Create a 4D vector out of a 3D one.
 * No default W component is specified and must be filled out.
 */
void vector3d_to_vector4d(const vector3d_t *vec, vector4d_t *out)
{
	out->x = vec->x;
	out->y = vec->y;
	out->z = vec->z;
}
