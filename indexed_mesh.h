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
void indexed_mesh_null(indexed_mesh_t *mesh);
int indexed_mesh_initialize(indexed_mesh_t *mesh, int num_vertices, int num_indices);
void indexed_mesh_destroy(indexed_mesh_t *mesh);

// Vertex handling.
const mesh_vertex_t* get_indexed_vertex(indexed_mesh_t *mesh, int index);

#endif // _INDEXED_MESH_H_