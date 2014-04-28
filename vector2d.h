#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

// Two-dimensional vector class.
typedef struct vector2d
{
	float x;
	float y;
} vector2d_t;

// Unary vector operations.
void vector2d_set(vector2d_t *vec, float x, float y);
void vector2d_clear(vector2d_t *vec);
float vector2d_magnitude(const vector2d_t *vec);
float vector2d_normalize(const vector2d_t *vec, vector2d_t *out);
void vector2d_scale(const vector2d_t *vec, float factor, vector2d_t *out);
void vector2d_negate(const vector2d_t *vec, vector2d_t *out);

// Binary vector operations.
void vector2d_copy(const vector2d_t *vec, vector2d_t *out);
void vector2d_add(const vector2d_t *a, const vector2d_t *b, vector2d_t *out);
void vector2d_subtract(const vector2d_t *a, const vector2d_t *b, vector2d_t *out);
float vector2d_dot_product(const vector2d_t *a, const vector2d_t *b);

// Hybrid operations.
void vector2d_scalar_add(const vector2d_t *base,
	float factor,
	const vector2d_t *scaled,
	vector2d_t *out);

#endif // _VECTOR_2D_H_