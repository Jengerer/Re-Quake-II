#pragma once

#include "common_define.h"

// Two dimensional vector class.
class CommonLibrary Vector2
{

public:

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2 &other);
	void operator=(const Vector2 &other);
	
	// Set all components to zero.
	void Clear();
	void Set(float x, float y);
	float GetSquareMagnitude() const;
	float GetMagnitude() const;

	// Store normalized vector in this vector.
	void Normalized(const Vector2 &vector);

	// Store scalar multiple of a vector in this vector.
	void ScalarMultiple(const Vector2 &vector, float factor);

	// Store negation of vector in this vector.
	void Negation(const Vector2 &vector);

	// Store sum of two vectors in thsi vector.
	void Sum(const Vector2 &a, const Vector2 &b);

	// Store the difference between two vectors in this vector.
	void Difference(const Vector2 &a, const Vector2 &b);

	// Get vector dot product.
	float DotProduct(const Vector2 &vector) const;

	// Compound binary operation for adding multiple of a vector to another vector.
	void SumMultiple(const Vector2 &base, const Vector2 &scaled, float factor);

private:

	// Constant representing dimension.
	static const int Dimension = 2;

public:

	float x, y;

};