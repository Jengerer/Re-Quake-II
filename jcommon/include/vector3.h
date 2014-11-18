#pragma once

#include "jcommon_define.h"

// Class representing a three dimensional vector/point or angle, in which the
// components are rotations about the corresponding axis (in the order X, Y, Z).
class CommonLibrary Vector3
{

public:

	Vector3();
	Vector3(float x, float y, float z);

	// Set all components to zero.
	void Clear();
	void Copy(const Vector3 *vector);
	void Set(float x, float y, float z);
	float GetSquareMagnitude() const;
	float GetMagnitude() const;

	// Store normalized vector in this vector.
	void Normalized(const Vector3 *vector);

	// Store scalar multiple of a vector in this vector.
	void ScalarMultiple(const Vector3 *vector, float factor);

	// Store negation of a vector in this vector.
	void Negation(const Vector3 *vector);

	// Store the sum of two vectors in this vector.
	void Sum(const Vector3 *a, const Vector3 *b);

	// Store the difference between two vectors in this vector.
	void Difference(const Vector3 *a, const Vector3 *b);

	// Get vector dot product.
	float DotProduct(const Vector3 *vector) const;

	// Store cross product of two vectors in this vector.
	void CrossProduct(const Vector3 *a, const Vector3 *b);

	// Compound operation of adding a scalar multiple of a vector to a vector.
	void SumMultiple(const Vector3 *base, const Vector3 *scaled, float factor);

	// Assign the angles to this vector corresponding to the given direction vector.
	// This vector cannot be the operand.
	void AnglesFromVector(const Vector3 *direction);

	// Output the direction vectors corresponding to the rotation represented by this angles-vector.
	void AnglesToVectors(Vector3 *forward, Vector3 *right, Vector3 *up) const;

private:

	// Constant representing dimension.
	static const int Dimension = 3;

public:

	float x, y, z;

};