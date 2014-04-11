#ifndef _WORLD_2D_H_
#define _WORLD_2D_H_

#include "aabb.h"

// 2D physics world.
typedef struct world2d
{
	object2d_t *objects;
	int object_count;
	int object_max;
} world2d_t;

// Collision detection probing.
typedef struct trace2d
{
	float fraction;
	object2d_t *hit_object;
	vector2d_t end_point;
} trace2d_t;

// World initialization and destruction.
int initialize_world(world2d_t *world, int object_count);
void destroy_world(world2d_t *world);

// Object management.
object2d_t *allocate_object(world2d_t *world);

// World simulation functions.
void trace_object(const world2d_t* world, const object2d_t *object,
	const vector2d_t *displacement, trace2d_t *out);

#endif // _WORLD_2D_H_
