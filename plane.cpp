#include "plane.h"

// Plane default constructor with no initialization.
Plane::Plane()
{
}

// Plane constructor by copied normal and distance.
Plane::Plane(const Vector3 *normal, float distance) : distance(distance)
{
	this->normal.Copy(normal);
}
