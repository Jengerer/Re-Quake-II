#ifndef _POLYGON_3D_H_
#define _POLYGON_3D_H_

#include "vector3d.h"

/*
 * Representing a polygon in which all vertices are coplanar.
 * Assumes the polygon has at least 3 vertices.
 * Assumes vertices are specified in clock-wise order.
 */
typedef struct polygon3d
{
	vector3d_t *vertices;
	int vertex_count;
	unsigned char *indices;
	int index_count;
} polygon3d_t;

// Polygon initialization/destruction.
void null_polygon(polygon3d_t *polygon);
int initialize_polygon(polygon3d_t *polygon, int vertex_count);
void destroy_polygon(polygon3d_t *polygon);

// Index handling.
int calculate_polygon_index_count(int vertex_count);
const vector3d_t *get_polygon_indexed_vertex(const polygon3d_t *polygon, unsigned char index);

// Polygon parameters.
void get_polygon_normal(const polygon3d_t *polygon, vector3d_t *out);

#endif // _POLYGON_3D_H_
