#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "polygon.h"
#include "vector3d.h"

/* Representing a physics object in a 2D world. */
typedef struct object
{
	vector3d_t origin;
	polygon_t polygon;

	// Intrusive list nodes.
	struct object *prev;
	struct object *next;
} object_t;

// Null a world polygon. 
void object_null(object_t *object);
int object_initialize(object_t *object);
void object_destroy(object_t *object);

#endif // _OBJECT_H_