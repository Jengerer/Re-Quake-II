#pragma once

#include <stdint.h>
#include <vector3.h>

// Structure for 3D short vector.
struct Vector3Short
{
	int16_t x;
	int16_t y;
	int16_t z;
};

// BSP plane structure.
struct BSPPlane
{
	Vector3 normal;
	float distance;
	uint32_t type;
};

// BSP tree node structure.
struct BSPNode
{
	// Splitting plane index.
	uint32_t plane;

	// Children indices.
	int32_t childFront;
	int32_t childBack;

	// Bounding box min and max points.
	Vector3Short bbox_min;
	Vector3Short bbox_max;

	// Faces index/length that are on this plane.
	uint16_t face_start;
	uint16_t face_count;
};

// BSP map structure.
struct BSPMap
{
	// Splitting plane array.
	BSPPlane *planes;
	int planeCount;
};