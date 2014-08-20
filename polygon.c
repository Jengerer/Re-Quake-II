#include <stdlib.h>
#include "polygon.h"

/*
 * Set the polygon to be of size 0 so it doesn't get destroyed improperly.
 */
void polygon_null(polygon_t *polygon)
{
	indexed_mesh_null(&polygon->indexed_mesh);
	renderer_null_model(&polygon->model);
}

/*
 * Allocate space for the vertices and indices.
 */
int polygon_initialize(polygon_t *polygon, int num_vertices)
{
	indexed_mesh_t *indexed_mesh;
	unsigned int *indices;
	unsigned char vertex;
	unsigned char index;
	int num_indices;
	
	// Allocate mesh.
	indexed_mesh = &polygon->indexed_mesh;
	num_indices = polygon_calculate_index_count(num_vertices);
	if (!indexed_mesh_initialize(indexed_mesh, num_vertices, num_indices)) {
		return 0;
	}
	indices = indexed_mesh->indices;

	// Fill in indices 3 at a time for each triangle.
	vertex = 1;
	for (index = 0; index < num_indices; index += 3) {
		// Assign (0, 1, 2), then (0, 2, 3), etc.
		indices[index] = 0;
		indices[index + 1] = vertex;
		indices[index + 2] = ++vertex;
	}
	return 1;
}

/*
 * Deallocate a polygon's vertices and indices.
 */
void polygon_destroy(polygon_t *polygon)
{
	indexed_mesh_destroy(&polygon->indexed_mesh);
}

/* Get a vertex from the polygon. */
mesh_vertex_t* polygon_get_vertex(polygon_t *polygon, int index)
{
	return indexed_mesh_get_vertex(&polygon->indexed_mesh, index);
}

/*
 * Calculate number of indices required for a given vertex count.
 */
int polygon_calculate_index_count(int num_vertices)
{
	// For 3 vertices, need 1 triangle, for 4 vertices, need 2 triangles, etc.
	return (num_vertices - 2) * 3;
}

/*
 * Calculate the front-facing normal for this polygon.
 * Fills out the normal in the polygon's structure.
 */
void polygon_calculate_plane(polygon_t *polygon)
{
	vector3d_t a;
	vector3d_t b;
	plane_t *plane;
	mesh_t *mesh;
	const vector3d_t *v0;
	const vector3d_t *v1;
	const vector3d_t *v2;

	// Get first 3 vertices.
	mesh = &polygon->indexed_mesh.mesh;
	plane = &polygon->plane;
	v0 = &mesh->vertices[0].position;
	v1 = &mesh->vertices[1].position;
	v2 = &mesh->vertices[2].position;

	// Calculate cross product of (v2 - v0) and (v1 - v0).
	vector3d_subtract(v1, v0, &a);
	vector3d_subtract(v2, v0, &b);
	vector3d_cross_product(&a, &b, &a);
	vector3d_normalize(&a, &plane->normal);

	// Get the distance by projecting any of the points onto the normal.
	plane->distance = vector3d_dot_product(v0, &plane->normal);
}

/* Initialize a rectangular polygon. */
int polygon_create_rectangle(polygon_t *polygon, float width, float height)
{
	float half_width;
	float half_height;
	mesh_vertex_t *vertex;

	// Initialize four vertices.
	if (!polygon_initialize(polygon, 4)) {
		return 0;
	}

	// Set up vertices.
	half_width = width / 2.0f;
	half_height = height / 2.0f;

	// Top left.
	vertex = polygon_get_vertex(polygon, 0);
	vector3d_set(&vertex->position, -half_width, -half_height, 0.0f);
	vector2d_set(&vertex->texture, 0.0f, 0.0f);
	
	// Top right.
	vertex = polygon_get_vertex(polygon, 1);
	vector3d_set(&vertex->position, half_width, -half_height, 0.0f);
	vector2d_set(&vertex->texture, 1.0f, 0.0f);

	// Bottom right.
	vertex = polygon_get_vertex(polygon, 2);
	vector3d_set(&vertex->position, half_width, half_height, 0.0f);
	vector2d_set(&vertex->texture, 1.0f, 1.0f);

	// Bottom left.
	vertex = polygon_get_vertex(polygon, 3);
	vector3d_set(&vertex->position, -half_width, half_height, 0.0f);
	vector2d_set(&vertex->texture, 0.0f, 1.0f);

	// Success.
	return 1;
}
