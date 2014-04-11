#ifndef _PLANE_H_
#define _PLANE_H_

#include "vector3d.h"

/*
 * Representing a plane by normal and distance from origin along that normal.
 */
typedef struct plane
{
	vector3d_t normal;
	float distance;
} plane_t;

#endif // _PLANE_H_
