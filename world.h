#ifndef _WORLD_H_
#define _WORLD_H_

#include "object.h"

/* Object representing a flat world in 3D. */
typedef struct world
{
	object_t *head;
} world_t;

// World initialization and destruction.
void world_null(world_t *world);
int world_initialize(world_t *world);
void world_destroy(world_t *world);

// World object addition and destruction.
object_t* world_create_object(world_t *world);
void world_destroy_object(world_t *world, object_t *object);

#endif // _WORLD_H_