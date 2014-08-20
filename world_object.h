#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_

#include "polygon.h"
#include "vector3d.h"

/* Representing a physics object in a 2D world. */
typedef struct world_object
{
	vector3d_t origin;
	polygon_t polygon;
} world_object_t;

// Null a world polygon. 
void world_object_null(world_object_t *object);
int world_object_initialize(world_object_t *object, int vertex_count);
void world_object_destroy(world_object_t *object);

#endif // _WORLD_OBJECT_H_