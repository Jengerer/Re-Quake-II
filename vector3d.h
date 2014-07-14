#ifndef _VECTOR_3D_H_
#define _VECTOR_3D_H_

// Generic 3D vector.
typedef struct vector3d
{
	float x;
	float y;
	float z;
} vector3d_t;

// Unary vector operations.
void vector3d_set(vector3d_t *vec, float x, float y, float z);
void vector3d_clear(vector3d_t *vec);
float vector3d_magnitude(const vector3d_t *vec);
float vector3d_normalize(const vector3d_t *vec, vector3d_t *out);
void vector3d_scale(const vector3d_t *vec, float factor, vector3d_t *out);
void vector3d_negate(const vector3d_t *vec, vector3d_t *out);

// Binary vector operations.
void vector3d_copy(const vector3d_t *vec, vector3d_t *out);
void vector3d_add(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);
void vector3d_subtract(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);
float vector3d_dot_product(const vector3d_t *a, const vector3d_t *b);
void vector3d_cross_product(const vector3d_t *a, const vector3d_t *b, vector3d_t *out);

// Operations for vectors as angles.
void vector3d_to_angles(const vector3d_t *vector, vector3d_t *out);
void angles_to_vector3d(
	const vector3d_t *angles,
	vector3d_t *forward,
	vector3d_t *right,
	vector3d_t *up);

// Hybrid operations.
void vector3d_scalar_add(const vector3d_t *base,
	float factor,
	const vector3d_t *scaled,
	vector3d_t *out);

#endif // _VECTOR_3D_H_
