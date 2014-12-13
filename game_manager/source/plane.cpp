#include "plane.h"

Plane::Plane()
{
	// No initialization.
}

Plane::Plane(const Vector3 &normal, float distance) : normal(normal), distance(distance)
{
}

Plane::Plane(const Plane &other) : normal(other.normal), distance(other.distance)
{
}
