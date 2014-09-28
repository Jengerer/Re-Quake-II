#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "renderer_interface.h"
#include "indexed_mesh.h"
#include "plane.h"

// Representing a polygon in which all vertices are coplanar.
// Assumes the polygon has at least 3 vertices.
// Assumes vertices are specified in clock-wise order.
typedef struct polygon
{
	// Mesh representing this polygon's shape.
	indexed_mesh_t indexed_mesh;

	// Plane that this polygon's vertices are on (calculated after filled).
	plane_t plane;
} polygon_t;

// Polygon initialization/destruction.
void polygon_null(polygon_t *polygon);
int polygon_initialize(polygon_t *polygon, int num_vertices);
void polygon_destroy(polygon_t *polygon);

// Polygon vertex functions.
int polygon_get_num_vertices(const polygon_t *polygon);
mesh_vertex_t* polygon_get_vertex(polygon_t *polygon, int index);

// Index handling.
int polygon_calculate_index_count(int num_vertices);

// Polygon parameters.
void polygon_calculate_plane(polygon_t *polygon);

// Miscellaneous polygon setting.
int polygon_create_rectangle(polygon_t *polygon, float width, float height);

#endif // _POLYGON_H_
