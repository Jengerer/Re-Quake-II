#include "vector3.h"
#include "math_common.h"

// Zero vector constant.
Vector3 Vector3::Zero(0.f, 0.f, 0.f);

Vector3::Vector3()
{
	// No initialization.
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z)
{
}

void Vector3::operator=(const Vector3 &other)
{
	Set(other.x, other.y, other.z);
}

// Set all components to zero.
void Vector3::Clear()
{
	Set(0.0f, 0.0f, 0.0f);
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
// This vector cannot be the operand.
void Vector3::AnglesFromVector(const Vector3 *direction)
{
	// Calculate yaw (rotation about Y axis).
	y = MathCommon::ArcTangent(direction->x, direction->z);

	// Calculate pitch (rotation about X axis).
	float squarexzDistance = (direction->x * direction->x) + (direction->z * direction->z);
	float xzDistance = MathCommon::SquareRoot(squarexzDistance);
	x = MathCommon::ArcTangent(direction->y, xzDistance);

	// Any roll will satisfy, so no roll.
	z = 0.0f;
}

// Turn this set of angles into direction vectors.
void Vector3::AnglesToVectors(Vector3 *forward, Vector3 *right, Vector3 *up) const
{
	float sinX, cosX;
	float sinY, cosY;
	float sinZ, cosZ;

	// Calculate common values.
	sinX = MathCommon::Sine(x);
	cosX = MathCommon::Cosine(x);
	sinY = MathCommon::Sine(y);
	cosY = MathCommon::Cosine(y);
	sinZ = MathCommon::Sine(z);
	cosZ = MathCommon::Cosine(z);

	// Fill out forward.
	if (forward != nullptr) {
		forward->x = -cosX * sinY;
		forward->y = sinX;
		forward->z = -cosX * cosY;
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
