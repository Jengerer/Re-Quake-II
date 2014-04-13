#ifndef _POLYGON_H_
#define _POLYGON_H_

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
	void *renderer_context;
} polygon_t;

// Polygon initialization/destruction.
void null_polygon(polygon_t *polygon);
int initialize_polygon(polygon_t *polygon, int vertex_count);
void destroy_polygon(polygon_t *polygon);

// Index handling.
int calculate_polygon_index_count(int vertex_count);

// Polygon parameters.
void calculate_polygon_plane(polygon_t *polygon);

#endif // _POLYGON_H_
