#pragma once

#include <vector3.h>

// Class representing a plane in 3-D.
// Represented by the normal and distance along it from the origin.
class Plane
{

public:

	Plane();
	Plane(const Vector3 &normal, float distance);
	Plane(const Plane &other);

public:

	Vector3 normal;
	float distance;

};
