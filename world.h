#ifndef _WORLD_H_
#define _WORLD_H_

#include "world_object.h"

/* Object representing a flat world in 3D. */
typedef struct world
{
	world_object_t *objects;
	int num_objects;
} world_t;

// World initialization and destruction.
void world_null(world_t *world);
int world_initialize(world_t *world, int num_objects);
void world_destroy(world_t *world);

#endif // _WORLD_H_