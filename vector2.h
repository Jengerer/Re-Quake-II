#pragma once

// Two dimensional vector class.
class Vector2
{

public:

	Vector2();
	Vector2(float x, float y);

// Parameter setting functions.
	
	// Set all components to zero.
	void Clear();
	// Copy components from another vector.
	void Copy(const Vector2 *vector);
	// Set all components of this vector.
	void Set(float x, float y);

// Length related functions.

	// Retrieve square of vector magnitude.
	float GetSquareMagnitude() const;
	// Retrieve vector length/magnitude.
	float GetMagnitude() const;

// Unary vector length/scale operations.

	// Store normalized vector in this vector.
	void Normalized(const Vector2 *vector);
	// Store scalar multiple of a vector in this vector.
	void ScalarMultiple(const Vector2 *vector, float factor);
	// Store negation of vector in this vector.
	void Negation(const Vector2 *vector);

// Binary vector operations.

	// Store sum of two vectors in thsi vector.
	void Sum(const Vector2 *a, const Vector2 *b);
	// Store the difference between two vectors in this vector.
	void Difference(const Vector2 *a, const Vector2 *b);
	// Get vector dot product.
	float DotProduct(const Vector2 *vector) const;
	// Compound binary operations.
	void SumMultiple(const Vector2 *base, const Vector2 *scaled, float factor);

public:

	float x, y;

};