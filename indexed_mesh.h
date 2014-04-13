#ifndef _INDEXED_MESH_H_
#define _INDEXED_MESH_H_

#include "mesh.h"

typedef struct indexed_mesh
{
	mesh_t mesh;
	unsigned int *indices;
	int num_indices;
} indexed_mesh_t;

// Indexed mesh initialization/destruction.
void null_indexed_mesh(indexed_mesh_t *mesh);
int initialize_indexed_mesh(indexed_mesh_t *mesh, int num_vertices, int num_indices);
void destroy_indexed_mesh(indexed_mesh_t *mesh);

// Vertex handling.
void get_indexed_vertex(indexed_mesh_t *mesh, int index, vector3d_t *out);

#endif // _INDEXED_MESH_H_