#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "vector3d.h"
#include "plane.h"

/*
 * Representing a polygon in which all vertices are coplanar.
 * Assumes the polygon has at least 3 vertices.
 * Assumes vertices are specified in clock-wise order.
 */
typedef struct polygon
{
	// Vertices and indices to them.
	vector3d_t *vertices;
	int vertex_count;
	unsigned char *indices;
	int index_count;

	// Plane that this polygon's vertices are on (calculated after filled).
	plane_t plane;
} polygon_t;

// Polygon initialization/destruction.
void null_polygon(polygon_t *polygon);
int initialize_polygon(polygon_t *polygon, int vertex_count);
void destroy_polygon(polygon_t *polygon);

// Index handling.
int calculate_polygon_index_count(int vertex_count);
const vector3d_t *get_polygon_indexed_vertex(const polygon_t *polygon, unsigned char index);

// Polygon parameters.
void calculate_polygon_plane(polygon_t *polygon);

#endif // _POLYGON_H_
