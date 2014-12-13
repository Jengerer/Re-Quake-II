#include "vector4.h"
#include "math_common.h"

Vector4::Vector4()
{
	// No initialization.
}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(const Vector4 &other) : x(other.x), y(other.y), z(other.z), w(other.w)
{
}

void Vector4::operator=(const Vector4 &other)
{
	Set(other.x, other.y, other.z, other.w);
}

// Set all components to zero.
void Vector4::Clear()
{
	Set(0.0f, 0.0f, 0.0f, 0.0f);
}

// Copy components from other vector.
void Vector4::Copy(const Vector4 *vector)
{
	Set(vector->x, vector->y, vector->z, vector->w);
}

// Set individual components.
void Vector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// Assign XYZ components for this 4D vector out of a 3D one.
// No default W component is specified and must be filled out.
void Vector4::FromVector3(const Vector3 *vector)
{
	x = vector->x;
	y = vector->y;
	z = vector->z;
}

// Convert a 4D homogeneous vector to a 3D vector.
// If the W component is zero, the X, Y, and Z components are unchanged.
void Vector4::ToVector3(Vector3 *outVector) const
{
	float scale;
	if (w == 0.0f) {
		scale = 1.0f;
	}
	else {
		scale = 1.0f / w;
	}
	outVector->x = x * scale;
	outVector->y = y * scale;
	outVector->z = z * scale;
}
