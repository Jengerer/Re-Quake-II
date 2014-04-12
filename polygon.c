#include "polygon.h"
#include <stdlib.h>

/*
 * Set the polygon to be of size 0 so it doesn't get destroyed improperly.
 */
void null_polygon(polygon_t *polygon)
{
	polygon->vertex_count = 0;
	polygon->index_count = 0;
}

/*
 * Allocate space for the vertices and indices.
 */
int initialize_polygon(polygon_t *polygon, int vertex_count)
{
	unsigned char vertex;
	unsigned char index;
	int index_count;
	unsigned char *indices; 
	vector3d_t *vertices;

	// Allocate vertices.
	vertices = (vector3d_t*)malloc(vertex_count * sizeof(vector3d_t));
	if (vertices == NULL) {
		return 0;
	}
	
	// Allocate indices.
	index_count = calculate_polygon_index_count(vertex_count);
	indices = (unsigned char*)malloc(index_count * sizeof(vector3d_t));
	if (indices == NULL) {
		free(vertices);
		return 0;
	}

	// Fill in indices 3 at a time for each triangle.
	vertex = 1;
	for (index = 0; index < index_count; index += 3) {
		// Assign (0, 1, 2), then (0, 2, 3), etc.
		indices[index] = 0;
		indices[index + 1] = vertex;
		indices[index + 2] = ++vertex;
	}

	// Fill out struct and return.
	polygon->vertices = vertices;
	polygon->vertex_count = vertex_count;
	polygon->indices = indices;
	polygon->index_count = index_count;
	return 1;
}

/*
 * Deallocate a polygon's vertices and indices.
 */
void destroy_polygon(polygon_t *polygon)
{
	if (polygon->vertex_count != 0) {
		free(polygon->vertices);
		polygon->vertex_count = 0;
	}
	if (polygon->index_count != 0) {
		free(polygon->indices);
		polygon->index_count = 0;
	}
}

/*
 * Calculate number of indices required for a given vertex count.
 */
int calculate_polygon_index_count(int vertex_count)
{
	// For 3 vertices, need 1 triangle, for 4 vertices, need 2 triangles, etc.
	return (vertex_count - 2) * 3;
}

/*
 * Get a vertex from polygon by index from index buffer.
 */
const vector3d_t *get_polygon_indexed_vertex(const polygon_t *polygon, unsigned char index)
{
	index = polygon->indices[index];
	return &polygon->vertices[index];
}

/*
 * Calculate the front-facing normal for this polygon.
 * Fills out the normal in the polygon's structure.
 */
void calculate_polygon_plane(polygon_t *polygon)
{
	vector3d_t a;
	vector3d_t b;
	plane_t *plane = &polygon->plane;
	const vector3d_t *v0 = &polygon->vertices[0];
	const vector3d_t *v1 = &polygon->vertices[1];
	const vector3d_t *v2 = &polygon->vertices[2];

	// Calculate cross product of (v2 - v0) and (v1 - v0).
	vector_subtract(v1, v0, &a);
	vector_subtract(v2, v0, &b);
	vector_cross_product(&a, &b, &a);
	vector_normalize(&a, &plane->normal);

	// Get the distance by projecting any of the points onto the normal.
	plane->distance = vector_dot_product(v0, &plane->normal);
}

