#pragma once

#include "common_define.h"
#include "vector3.h"
#include "vector4.h"

// Class for representing a 4x4 matrix.
// Any operations for which this class cannot be an operand are explicitly stated.
class CommonLibrary Matrix4x4
{

public:

	// Load identity matrix into this matrix.
	void Identity();

	// Assign this matrix the result of a matrix transpose.
	void Transpose(const Matrix4x4 *matrix);

	// Assign this matrix the result of a matrix multiplication.
	// This matrix cannot be one of the operands.
	void Product(const Matrix4x4 *a, const Matrix4x4 *b);

	// Load a translation matrix.
	void Translation(const Vector3 *translation);

	// Load a rotation about the X axis.
	void RotationX(float angle);

	// Load a rotation about the Y axis.
	void RotationY(float angle);

	// Load a rotation about the Z axis.
	void RotationZ(float angle);

	// Load a full rotation about a set of Euler angles.
	void RotationEuler(const Vector3 *angles);

	// Load a perspective projection by frustum parameters.
	void PerspectiveProjectionFrustum(
		float left,
		float right,
		float top,
		float bottom,
		float zNear,
		float zFar);

	// Load a perspective projection by aspect ratio, field of view, and near/far planes.
	void PerspectiveProjection(
		float aspectRatio,
		float fieldOfView,
		float zNear,
		float zFar);

	// Transform a vector by this matrix.
	void Transform(const Vector4 *vector, Vector4 *out);

private:

	// Helper constant for duplicating code.
	static const int Size = 4;

public:

	float matrixArray[Size][Size];

};