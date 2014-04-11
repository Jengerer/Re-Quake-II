#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

// Generic 3D vector.
typedef struct vector3d
{
	float x;
	float y;
	float z;
} vector3d_t;

// Unary vector operations.
void vector_set(vector3d_t *vec, float x, float y, float z);
void vector_clear(vector3d_t *vec);
float vector_magnitude(const vector3d_t *vec);
float vector_normalize(const vector3d_t *vec, vector3d_t *out);
void vector_scale(const vector3d_t *vec, float factor, vector3d_t *out);
void vector_negate(const vector3d_t *vec, vector3d_t *out);

// Binary vector operations.
void vector_copy(const vector3d_t *vec, vector3d_t *out);
void vector_add(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);
void vector_subtract(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);
float vector_dot_product(const vector3d_t *a, const vector3d_t *b);
void vector_cross_product(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);

#endif // _VECTOR_2D_H_
