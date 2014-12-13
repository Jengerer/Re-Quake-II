#include "vector2.h"
#include "math_common.h"

Vector2::Vector2()
{
	// No initialization.
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector2::Vector2(const Vector2 &other) : x(other.x), y(other.y)
{
}

void Vector2::operator=(const Vector2 &other)
{
	Set(other.x, other.y);
}

// Set all components to zero.
void Vector2::Clear()
{
	Set(0.0f, 0.0f);
}

// Set individual components.
void Vector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

// Get the vector L2-norm (square magnitude).
float Vector2::GetSquareMagnitude() const
{
	return DotProduct(this);
}

// Get the vector magnitude.
float Vector2::GetMagnitude() const
{
	float SquareMagnitude = GetSquareMagnitude();
	return MathCommon::SquareRoot(SquareMagnitude);
}

// Assign normalized vector.
void Vector2::Normalized(const Vector2 *vector)
{
	float inverseMagnitude = 1.0f / GetMagnitude();
	ScalarMultiple(vector, inverseMagnitude);
}

// Assign scalar multiple of vector.
void Vector2::ScalarMultiple(const Vector2 *vector, float factor)
{
	Set(vector->x * factor, vector->y * factor);
}

// Assign vector negation.
void Vector2::Negation(const Vector2 *vector)
{
	Set(-vector->x, -vector->y);
}

// Assign result of vector sum.
void Vector2::Sum(const Vector2 *a, const Vector2 *b)
{
	Set(a->x + b->x, a->y + b->y);
}

// Assign result of vector difference.
void Vector2::Difference(const Vector2 *a, const Vector2 *b)
{
	Set(a->x - b->x, a->y - b->y);
}

// Calculate vector dot product.
float Vector2::DotProduct(const Vector2 *vector) const
{
	return (x * vector->x) + (y * vector->y);
}

// Assign sum with scaled vector.
void Vector2::SumMultiple(const Vector2 *base, const Vector2 *scaled, float factor)
{
	ScalarMultiple(scaled, factor);
	Sum(base, this);
}
