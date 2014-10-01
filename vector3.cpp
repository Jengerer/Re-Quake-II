#include "vector3.h"
#include "math_common.h"

// Vector default constructor does not set any elements.
Vector3::Vector3()
{
}

// Vector constructor with default component values.
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

// Set all components to zero.
void Vector3::Clear()
{
	Set(0.0f, 0.0f, 0.0f);
}

// Copy components from other vector.
void Vector3::Copy(const Vector3 *vector)
{
	Set(vector->x, vector->y, vector->z);
}

// Set individual components.
void Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// Get the vector L2-norm (square magnitude).
float Vector3::GetSquareMagnitude() const
{
	return DotProduct(this);
}

// Get the vector magnitude.
float Vector3::GetMagnitude() const
{
	float SquareMagnitude = GetSquareMagnitude();
	return MathCommon::SquareRoot(SquareMagnitude);
}

// Assign normalized vector.
void Vector3::Normalized(const Vector3 *vector)
{
	float inverseMagnitude = 1.0f / GetMagnitude();
	ScalarMultiple(vector, inverseMagnitude);
}

// Assign scalar multiple of vector.
void Vector3::ScalarMultiple(const Vector3 *vector, float factor)
{
	x = vector->x * factor;
	y = vector->y * factor;
	z = vector->z * factor;
}

// Assign vector negation.
void Vector3::Negation(const Vector3 *vector)
{
	x = -(vector->x);
	y = -(vector->y);
	z = -(vector->z);
}

// Assign result of vector sum.
void Vector3::Sum(const Vector3 *a, const Vector3 *b)
{
	x = a->x + b->x;
	y = a->y + b->y;
	z = a->z + b->z;
}

// Assign result of vector difference.
void Vector3::Difference(const Vector3 *a, const Vector3 *b)
{
	x = a->x - b->x;
	y = a->y - b->y;
	z = a->z - b->z;
}

// Calculate vector dot product.
float Vector3::DotProduct(const Vector3 *vector) const
{
	return (x * vector->x) + (y * vector->y) + (z * vector->z);
}

// Assign result of vector cross product to this vector.
void Vector3::CrossProduct(const Vector3 *a, const Vector3 *b)
{
	// Store X and Y in variables since we need originals to calculate Z.
	float xProduct = (a->y * b->z) - (a->z * b->y);
	float yProduct = (a->z * b->x) - (a->x * b->z);

	// Calculate Z and fill in results.
	z = (a->x * b->y) - (a->y * b->x);
	y = yProduct;
	x = xProduct;
}

// Assign sum with scaled vector.
void Vector3::SumMultiple(const Vector3 *base, const Vector3 *scaled, float factor)
{
	ScalarMultiple(scaled, factor);
	Sum(base, this);
}

// Sets components to a set of degree angles matching a given direction.
// Assumes input vector is not the zero vector.
void Vector3::AnglesFromVector(const Vector3 *direction)
{
	float xzDistance;
	float xzArcTangent;
	float yzArcTangent;

	// Calculate yaw (rotation about Y axis).
	xzArcTangent = MathCommon::ArcTangent(direction->x, direction->z);
	y = MathCommon::RadiansToDegrees(xzArcTangent);

	// Calculate pitch (rotation about X axis).
	float squarexzDistance = (direction->x * direction->x) + (direction->z * direction->z);
	xzDistance = MathCommon::SquareRoot(squarexzDistance);
	yzArcTangent = MathCommon::ArcTangent(direction->y, xzDistance);
	x = MathCommon::RadiansToDegrees(yzArcTangent);

	// Any roll will satisfy, so no roll.
	z = 0.0f;
}

// Turn this set of angles into direction vectors.
void Vector3::AnglesToVectors(Vector3 *forward, Vector3 *right, Vector3 *up) const
{
	float xRadians, yRadians, zRadians;
	float sinX, cosX;
	float sinY, cosY;
	float sinZ, cosZ;

	// Convert angles.
	xRadians = MathCommon::DegreesToRadians(x);
	yRadians = MathCommon::DegreesToRadians(y);
	zRadians = MathCommon::DegreesToRadians(z);

	// Calculate common values.
	sinX = MathCommon::Sine(xRadians);
	cosX = MathCommon::Cosine(xRadians);
	sinY = MathCommon::Sine(yRadians);
	cosY = MathCommon::Cosine(yRadians);
	sinZ = MathCommon::Sine(zRadians);
	cosZ = MathCommon::Cosine(zRadians);

	// Fill out forward.
	if (forward != nullptr) {
		forward->x = cosX * sinY;
		forward->y = -sinX;
		forward->z = cosX * cosY;
	}

	// Fill out right.
	if (right != nullptr) {
		right->x = (sinX * sinY * sinZ) + (cosY * cosZ);
		right->y = (cosX * sinZ);
		right->z = (sinX * cosY * sinZ) - (sinY * cosZ);
	}

	// Fill out up.
	if (up != nullptr) {
		up->x = (sinX * sinY * cosZ) - (cosY * sinZ);
		up->y = cosX * cosZ;
		up->z = (sinY * sinZ) + (sinX * cosY * cosZ);
	}
}