#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "renderer.h"
#include "indexed_mesh.h"
#include "plane.h"

/*
 * Representing a polygon in which all vertices are coplanar.
 * Assumes the polygon has at least 3 vertices.
 * Assumes vertices are specified in clock-wise order.
 */
typedef struct polygon
{
	// Mesh representing this polygon's shape.
	indexed_mesh_t indexed_mesh;

	// Plane that this polygon's vertices are on (calculated after filled).
	plane_t plane;

	// Render context for this polygon.
	renderer_model_t model;
} polygon_t;

// Polygon initialization/destruction.
void polygon_null(polygon_t *polygon);
int polygon_initialize(polygon_t *polygon, int num_vertices);
void polygon_destroy(polygon_t *polygon);

// Index handling.
int polygon_calculate_index_count(int num_vertices);

// Polygon parameters.
void polygon_calculate_plane(polygon_t *polygon);

#endif // _POLYGON_H_
