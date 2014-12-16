#include "plane.h"

namespace Geometry
{

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

	// Returns whether a point is in front of the plane.
	bool Plane::IsPointInFront(const Vector3 &point) const
	{
		float offset = normal.DotProduct(point) - distance;
		return (offset > 0.f);
	}

}