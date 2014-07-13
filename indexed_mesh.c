#include "indexed_mesh.h"
#include "memory_manager.h"

/*
 * Null the struct for safe destruction.
 */
void null_indexed_mesh(indexed_mesh_t *mesh)
{
	mesh->num_indices = 0;
}

/*
 * Initialize the vertex and index buffers.
 */
int initialize_indexed_mesh(indexed_mesh_t *mesh, int num_vertices, int num_indices)
{
	unsigned int *indices;

	null_indexed_mesh(mesh);

	// Initialize base mesh.
	null_mesh(&mesh->mesh);
	if (!initialize_mesh(&mesh->mesh, num_vertices)) {
		return 0;
	}

	// Allocate for indices.
	indices = (unsigned int*)memory_allocate(num_indices * sizeof(unsigned int));
	if (indices == NULL) {
		return 0;
	}
	mesh->indices = indices;
	mesh->num_indices = num_indices;
	return 1;
}

/*
 * Destroy the index and vertex buffers.
 */
void destroy_indexed_mesh(indexed_mesh_t *mesh)
{
	destroy_mesh(&mesh->mesh);

	// Free index buffer if we have one.
	if (mesh->num_indices != 0) {
		memory_free(mesh->indices);
	}
	null_indexed_mesh(mesh);
}

/*
 * Get an vector by its index in the index buffer.
 */
const mesh_vertex_t* get_indexed_vertex(indexed_mesh_t *mesh, int index)
{
	index = mesh->indices[index];
	return &mesh->mesh.vertices[index];
}
