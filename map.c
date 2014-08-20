#include "map.h"
#include "memory_manager.h"
#include <stdio.h> // TODO: Maybe remove.

/*
 * Base initialization to have no polygons.
 */
void null_map(map_t *map)
{
	map->num_polygons = 0;
}

/*
 * Allocate space for a certain number of polygons.
 */
int initialize_map(map_t *map, int num_polygons)
{
	int i;
	polygon_t *polygons;

	// Null map first in case something fails here.
	null_map(map);

	// Allocate space for polygons and null them.
	polygons = (polygon_t*)memory_allocate(num_polygons * sizeof(polygon_t));
	if (polygons == NULL) {
		return 0;
	}
	for (i = 0; i < num_polygons; ++i) {
		polygon_null(&polygons[i]);
	}

	// Fill out struct and return.
	map->polygons = polygons;
	map->num_polygons = num_polygons;
	return 1;
}

/*
 * Deallocate all polygons.
 */
void destroy_map(map_t *map)
{
	int i;
	for (i = 0; i < map->num_polygons; ++i) {
		polygon_destroy(&map->polygons[i]);
	}
	memory_free(map->polygons);
	null_map(map);
}

/*
 * Trace an AABB movement through the map and return collision information.
 */
void trace_aabb(const map_t *map,
	const aabb_t *aabb,
	const vector3d_t *start,
	const vector3d_t *end,
	trace_t *trace)
{
	int i;
	float start_dot, end_dot, aabb_dot;
	float fraction;
	const plane_t *plane;
	const polygon_t *polygon;

	// Initialize base trace parameters.
	trace->fraction = 1.0f;

	// Go through each polygon.
	for (i = 0; i < map->num_polygons; ++i) {
		polygon = &map->polygons[i];
		plane = &polygon->plane;

		// Project the AABB onto the plane's normal.
		aabb_dot = get_aabb_projection(aabb, plane);
		start_dot = vector3d_dot_product(start, &plane->normal) + aabb_dot;
		end_dot = vector3d_dot_product(end, &plane->normal) + aabb_dot;

		// No collision of both start and end are on one side of plane.
		if ((start_dot > 0.0f) && (end_dot >= start_dot)) {
			continue;
		}
		if ((start_dot <= 0.0f) && (end_dot <= 0.0f)) {
			continue;
		}

		// Get crossing time.
		fraction = start_dot / (start_dot - end_dot);
		if ((fraction >= 0.0f) && (fraction <= 1.0f)) {
			vector3d_t direction;
			vector3d_t new_end;
			vector3d_subtract(end, start, &direction);
			vector3d_scalar_add(start, fraction, &direction, &new_end);
		}
	}
}

/*
 * Get the relevant projection of AABB to the plane.
 * If normal points in the positive coordinate direction, we need
 * the minimum coordinate from the AABB, and vice-versa.
 */
float get_aabb_projection(const aabb_t *aabb, const plane_t *plane)
{
	vector3d_t offsets;
	const vector3d_t *normal = &plane->normal;
	offsets.x = (normal->x > 0.0f ? aabb->mins.x : aabb->maxs.x);
	offsets.y = (normal->y > 0.0f ? aabb->mins.y : aabb->maxs.y);
	offsets.z = (normal->z > 0.0f ? aabb->mins.z : aabb->maxs.z);
	return vector3d_dot_product(&offsets, normal) - plane->distance;
}

