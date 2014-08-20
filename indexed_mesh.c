#include "indexed_mesh.h"
#include "memory_manager.h"

/*
 * Null the struct for safe destruction.
 */
void indexed_mesh_null(indexed_mesh_t *mesh)
{
	mesh_null(&mesh->mesh);
	mesh->num_indices = 0;
}

/*
 * Initialize the vertex and index buffers.
 */
int indexed_mesh_initialize(indexed_mesh_t *mesh, int num_vertices, int num_indices)
{
	unsigned int *indices;

	// Initialize base mesh.
	if (!mesh_initialize(&mesh->mesh, num_vertices)) {
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
void indexed_mesh_destroy(indexed_mesh_t *mesh)
{
	mesh_destroy(&mesh->mesh);

	// Free index buffer if we have one.
	if (mesh->num_indices != 0) {
		memory_free(mesh->indices);
	}
	indexed_mesh_null(mesh);
}

/*
 * Get an vector by its index in the index buffer.
 */
const mesh_vertex_t* get_indexed_vertex(indexed_mesh_t *mesh, int index)
{
	index = mesh->indices[index];
	return &mesh->mesh.vertices[index];
}
