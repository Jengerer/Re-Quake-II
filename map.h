#ifndef _MAP_H_
#define _MAP_H_

#include "object.h"
#include "trace.h"

/*
 * A map represented by a number of polygons.
 */
typedef struct map
{
	object_t *objects;
	int num_objects;
} map_t;

// Map initialization and destruction.
void null_map(map_t *map);
int initialize_map(map_t *map, int num_objects);
void destroy_map(map_t *map);

#endif // _MAP_H_
