#ifndef _WORLD_H_
#define _WORLD_H_

#include "map.h"
#include "object.h"
#include "vector3d.h"

/* Object representing a flat world in 3D. */
typedef struct world
{
	// Moving physics objects.
	dynamic_object_t *dynamic_head;

	// Static physics objects.
	map_t map;
} world_t;

// World initialization and destruction.
void world_null(world_t *world);
int world_initialize(world_t *world);
void world_destroy(world_t *world);

// World object addition and destruction.
dynamic_object_t* world_create_dynamic_object(world_t *world);
void world_destroy_dynamic_object(world_t *world, dynamic_object_t *dynamic);

#endif // _WORLD_H_