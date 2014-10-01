#pragma once

#include "vector3.h"

// Class representing a plane in 3-D.
// Represented by the normal and distance along it from the origin.
class Plane
{
public:

	// Default constructor that sets no elements.
	Plane();
	// Constructor by copied normal and distance.
	Plane(const Vector3 *normal, float distance);

public:

	Vector3 normal;
	float distance;

};
