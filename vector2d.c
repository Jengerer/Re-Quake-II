#include "vector2d.h"
#include <math.h>

/*
 * Set all vector components.
 */
void vector2d_set(vector2d_t *vec, float x, float y)
{
	vec->x = x;
	vec->y = y;
}

/*
 * Clear a vector's components.
 */
void vector2d_clear(vector2d_t *vec)
{
	vector2d_set(vec, 0.0f, 0.0f);
}

/*
 * Return a vector's magnitude.
 */
float vector2d_magnitude(const vector2d_t *vec)
{
	float squared = vector2d_dot_product(vec, vec);
	return sqrtf(squared);
}

/*
 * Calculate unit vector with same direction as vector.
 * Assumes the vector has non-zero magnitude.
 * Returns the vector's magnitude.
 */
float vector2d_normalize(const vector2d_t *vec, vector2d_t *out)
{
	float magnitude = vector2d_magnitude(vec);
	float inverse = 1.0f / magnitude;
	vector2d_scale(vec, inverse, out);
	return magnitude;
}

/*
 * Calculate scalar multiple of a vector.
 */
void vector2d_scale(const vector2d_t *vec, float factor, vector2d_t *out)
{
	out->x = vec->x * factor;
	out->y = vec->y * factor;
}

/*
 * Calculate vector in the opposite direction.
 */
void vector2d_negate(const vector2d_t *vec, vector2d_t *out)
{
	out->x = -vec->x;
	out->y = -vec->y;
}

/*
 * Copy a vector to another.
 */
void vector2d_copy(const vector2d_t *vec, vector2d_t *out)
{
	out->x = vec->x;
	out->y = vec->y;
}

/*
 * Calculate vector sum (a + b).
 * Assigns out to the value of a + b.
 */
void vector2d_add(const vector2d_t *a, const vector2d_t *b, vector2d_t *out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
}

/*
 * Calculate vector difference.
 * Assigns out to the value of a - b.
 */
void vector2d_subtract(const vector2d_t *a, const vector2d_t *b, vector2d_t *out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}

/*
 * Calculate and return vector dot product.
 */
float vector2d_dot_product(const vector2d_t *a, const vector2d_t *b)
{
	return (a->x * b->x) + (a->y * b->y);
}

/*
 * Add a scalar multiple of some vector to another.
 */
void vector2d_scalar_add(const vector2d_t *base,
	float factor,
	const vector2d_t *scaled,
	vector2d_t *out)
{
	vector2d_scale(scaled, factor, out);
	vector2d_add(base, out, out);
}
