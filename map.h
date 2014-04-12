#ifndef _MAP_H_
#define _MAP_H_

#include "aabb.h"
#include "polygon.h"
#include "trace.h"

/*
 * A map represented by a number of polygons.
 */
typedef struct map
{
	polygon_t *polygons;
	int polygon_count;
} map_t;

// Map initialization and destruction.
void null_map(map_t *map);
int initialize_map(map_t *map, int polygon_count);
void destroy_map(map_t *map);

// Map collision tracing.
void trace_aabb(const map_t *map,
	const aabb_t *aabb,
	const vector3d_t *start,
	const vector3d_t *end,
	trace_t *trace);

// Auxiliary collision functions.
float get_aabb_projection(const aabb_t *aabb, const plane_t *plane);

#endif // _MAP_H_
