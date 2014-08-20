#ifndef _VECTOR_4D_H_
#define _VECTOR_4D_H_

#include "vector3d.h"

// Generic 4D vector.
typedef struct vector4d
{
	float x;
	float y;
	float z;
	float w;
} vector4d_t;

// Unary vector operations.
void vector4d_set(vector4d_t *vec, float x, float y, float z, float w);
void vector4d_clear(vector4d_t *vec);

// Binary vector operations.
void vector4d_copy(const vector4d_t *vec, vector4d_t *out);

// Vector conversions.
void vector4d_to_vector3d(const vector4d_t *vec, vector3d_t *out);
void vector3d_to_vector4d(const vector3d_t *vec, vector4d_t *out);

#endif // _VECTOR_4D_H_