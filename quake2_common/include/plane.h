#pragma once

#include <vector3.h>

namespace Geometry
{

	// Class representing a plane in 3-D.
	// Represented by the normal and distance along it from the origin.
	class Plane
	{

	public:

		Plane();
		Plane(const Vector3 &normal, float distance);
		Plane(const Plane &other);

		// Check if a point is in front of the plane.
		bool IsPointInFront(const Vector3 &point) const;

	public:

		Vector3 normal;
		float distance;

	};

}