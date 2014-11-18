#pragma once

#include "common_define.h"
#include "vector3.h"

// Class representing a three dimensional vector/point or angle, in which the
// components are rotations about the corresponding axis (in the order X, Y, Z).
class CommonLibrary Vector4
{

public:

	Vector4();
	Vector4(float x, float y, float z, float w);

	// Set all components to zero.
	void Clear();
	void Copy(const Vector4 *vector);
	void Set(float x, float y, float z, float w);

	// Set the XYZ components of this vector from a 3-dimensional vector.
	void FromVector3(const Vector3 *vector);

	// Convert from homogeneous coordinates to 3-dimensional vector.
	void ToVector3(Vector3 *out) const;

private:

	// Constant representing dimension.
	static const int Dimension = 4;

public:
		
	float x, y, z, w;

};