#ifndef _MAP_H_
#define _MAP_H_

#include "object.h"

/* Map element node. */
typedef struct map_object
{
	object_t object;

	// Linked list nodes.
	struct map_object *next;
	struct map_object *prev;
} map_object_t;

/*
 * A map represented by a number of polygons.
 */
typedef struct map
{
	map_object_t *head;
} map_t;

// Map initialization and destruction.
void map_null(map_t *map);
int map_initialize(map_t *map);
void map_destroy(map_t *map);

#endif // _MAP_H_
