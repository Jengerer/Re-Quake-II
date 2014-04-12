#include "map.h"
#include <stdio.h> // TODO: Maybe remove.
#include <stdlib.h>

/*
 * Base initialization to have no polygons.
 */
void null_map(map_t *map)
{
	map->polygon_count = 0;
}

/*
 * Allocate space for a certain number of polygons.
 */
int initialize_map(map_t *map, int polygon_count)
{
	int i;
	polygon_t *polygons;

	// Null map first in case something fails here.
	null_map(map);

	// Allocate space for polygons and null them.
	polygons = (polygon_t*)malloc(polygon_count * sizeof(polygon_t));
	if (polygons == NULL) {
		return 0;
	}
	for (i = 0; i < polygon_count; ++i) {
		null_polygon(&polygons[i]);
	}

	// Fill out struct and return.
	map->polygons = polygons;
	map->polygon_count = polygon_count;
	return 1;
}

/*
 * Deallocate all polygons.
 */
void destroy_map(map_t *map)
{
	int i;
	for (i = 0; i < map->polygon_count; ++i) {
		destroy_polygon(&map->polygons[i]);
	}
	map->polygon_count = 0;
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
	float start_dot;
	float end_dot;
	float aabb_dot;
	float fraction;
	const plane_t *plane;
	const polygon_t *polygon;

	// Initialize base trace parameters.
	trace->fraction = 1.0f;

	// Go through each polygon.
	for (i = 0; i < map->polygon_count; ++i) {
		polygon = &map->polygons[i];
		plane = &polygon->plane;

		// Project the AABB onto the plane's normal.
		aabb_dot = get_aabb_projection(aabb, plane);
		start_dot = vector_dot_product(start, &plane->normal) + aabb_dot;
		end_dot = vector_dot_product(end, &plane->normal) + aabb_dot;

		// No collision of both start and end are on one side of plane.
		if (start_dot > 0.0f && end_dot >= start_dot) {
			continue;
		}
		if (start_dot <= 0.0f && end_dot <= 0.0f) {
			continue;
		}

		// Get crossing time.
		fraction = start_dot / (start_dot - end_dot);
		if ((fraction >= 0.0f) && (fraction <= 1.0f)) {
			vector3d_t direction;
			vector3d_t new_end;
			vector_subtract(end, start, &direction);
			vector_scalar_add(start, fraction, &direction, &new_end);
			printf("Fraction: %f\n", fraction);
			printf("End at: (%f, %f, %f)\n", new_end.x, new_end.y, new_end.z);
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
	return vector_dot_product(&offsets, normal) - plane->distance;
}

