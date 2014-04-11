#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

typedef struct vector2d
{
	float x;
	float y;
} vector2d_t;

// Unary vector operations.
void vector_set(vector2d_t *vec, float x, float y);
void vector_clear(vector2d_t *vec);
float vector_magnitude(const vector2d_t *vec);
float vector_normalize(const vector2d_t *vec, vector2d_t *out);
void vector_scale(const vector2d_t *vec, float factor, vector2d_t *out);
void vector_negate(const vector2d_t *vec, vector2d_t *out);

// Binary vector operations.
void vector_copy(const vector2d_t *vec, vector2d_t *out);
void vector_add(const vector2d_t *a, const vector2d_t *b, vector2d_t *out);
void vector_subtract(const vector2d_t *a, const vector2d_t *b, vector2d_t *out);
float vector_dot_product(const vector2d_t *a, const vector2d_t *b);

#endif // _VECTOR_2D_H_
