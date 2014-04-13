#include "mesh.h"
#include <stdlib.h>

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
	vector3d_t *vertices = (vector3d_t*)malloc(num_vertices * sizeof(vector3d_t));
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
		free(mesh->vertices);
	}
}
