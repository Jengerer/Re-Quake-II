#pragma once

// Class representing a three dimensional vector/point or angle, in which the
// components are rotations about the corresponding axis (in the order X, Y, Z).
class Vector3
{

public:

	Vector3();
	Vector3(float x, float y, float z);

// Parameter setting functions.

	// Set all components to zero.
	void Clear();
	// Copy components from another vector.
	void Copy(const Vector3 *vector);
	// Set all components of this vector.
	void Set(float x, float y, float z);

// Vector length related functions.

	// Retrieve square of vector magnitude.
	float GetSquareMagnitude() const;
	// Retrieve vector length/magnitude.
	float GetMagnitude() const;

// Unary vector length/scale operations.

	// Store normalized vector in this vector.
	void Normalized(const Vector3 *vector);
	// Store scalar multiple of a vector in this vector.
	void ScalarMultiple(const Vector3 *vector, float factor);
	// Store negation of a vector in this vector.
	void Negation(const Vector3 *vector);

// Binary vector operations.

	// Store the sum of two vectors in this vector.
	void Sum(const Vector3 *a, const Vector3 *b);
	// Store the difference between two vectors in this vector.
	void Difference(const Vector3 *a, const Vector3 *b);
	// Get vector dot product.
	float DotProduct(const Vector3 *vector) const;
	// Store cross product of two vectors in this vector.
	void CrossProduct(const Vector3 *a, const Vector3 *b);
	// Compound binary operations.
	void SumMultiple(const Vector3 *base, const Vector3 *scaled, float factor);

// Functions for handling angles as a vector.

	// Assign the angles to this vector corresponding to the given direction vector.
	void AnglesFromVector(const Vector3 *direction);
	// Assign to this vector the unit vector matching the given angle orientation.
	void AnglesToVectors(Vector3 *forward, Vector3 *right, Vector3 *up) const;

public:

	float x, y, z;

};