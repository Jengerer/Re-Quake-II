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
 * Get vertex by index. 
 * Note: this is the index in the mesh, not by the indexed mesh indices.
 */
mesh_vertex_t* indexed_mesh_get_vertex(indexed_mesh_t *indexed_mesh, int index)
{
	return mesh_get_vertex(&indexed_mesh->mesh, index);
}
