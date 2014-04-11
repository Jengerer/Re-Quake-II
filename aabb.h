#ifndef _AABB_H_
#define _AABB_H_

#include "vector3d.h"

/*
 * Class for representing a 3D axially-aligned bounding box.
 */
typedef struct aabb
{
	vector3d_t position;
	vector3d_t mins;
	vector3d_t maxs;
} aabb_t;

#endif // _AABB_H_
