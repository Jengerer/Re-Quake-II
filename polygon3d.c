#include "polygon3d.h"
#include <stdlib.h>

/*
 * Set the polygon to be of size 0 so it doesn't get destroyed improperly.
 */
void null_polygon(polygon3d_t *polygon)
{
	polygon->vertex_count = 0;
	polygon->index_count = 0;
}

/*
 * Allocate space for the vertices and indices.
 */
int initialize_polygon(polygon3d_t *polygon, int vertex_count)
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
void destroy_polygon(polygon3d_t *polygon)
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
const vector3d_t *get_polygon_vertex(const polygon3d_t *polygon, unsigned char index)
{
	index = polygon->indices[index];
	return &polygon->vertices[index];
}

