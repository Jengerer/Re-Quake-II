#include "mesh.h"
#include "memory_manager.h"

/*
 * Null the mesh for safe destruction.
 */
void mesh_null(mesh_t *mesh)
{
	mesh->num_vertices = 0;
}

/*
 * Allocate space for vertices.
 */
int mesh_initialize(mesh_t *mesh, int num_vertices)
{
	mesh_vertex_t *vertices = (mesh_vertex_t*)memory_allocate(num_vertices * sizeof(mesh_vertex_t));
	if (vertices == NULL) {
		return 0;
	}
	mesh->vertices = vertices;
	mesh->num_vertices = num_vertices;
	return 1;
}

/*
 * Destroy a mesh.
 */
void mesh_destroy(mesh_t *mesh)
{
	if (mesh->num_vertices != 0) {
		memory_free(mesh->vertices);
	}
}

/* Get a vertex by index. */
mesh_vertex_t* mesh_get_vertex(mesh_t *mesh, int index)
{
	return &mesh->vertices[index];
}
