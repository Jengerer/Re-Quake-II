#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "polygon.h"
#include "vector3d.h"

/* Representing a physics object in a 2D world. */
typedef struct object
{
	vector3d_t origin;
	polygon_t polygon;
} object_t;

// Null a world polygon. 
void object_null(object_t *object);
int object_initialize(object_t *object);
void object_destroy(object_t *object);

/* Structure for dynamic world object. */
typedef struct dynamic_object
{
	object_t object;
	vector3d_t velocity;

	// Linked list nodes.
	struct dynamic_object *next;
	struct dynamic_object *prev;
} dynamic_object_t;

// Dynamic object initialization and destruction.
void dynamic_object_null(dynamic_object_t *dynamic);
int dynamic_object_initialize(dynamic_object_t *dynamic);
void dynamic_object_destroy(dynamic_object_t *dynamic);

#endif // _OBJECT_H_