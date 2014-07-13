#include "mesh.h"
#include "memory_manager.h"

/*
 * Null the mesh for safe destruction.
 */
void null_mesh(mesh_t *mesh)
{
	mesh->num_vertices = 0;
}

/*
 * Allocate space for vertices.
 */
int initialize_mesh(mesh_t *mesh, int num_vertices)
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
void destroy_mesh(mesh_t *mesh)
{
	if (mesh->num_vertices != 0) {
		memory_free(mesh->vertices);
	}
}
