#ifndef _MESH_H_
#define _MESH_H_

#include "vector3d.h"

// Generic mesh of vertices.
typedef struct mesh
{
	// Vertices and indices to them.
	vector3d_t *vertices;
	int num_vertices;
} mesh_t;

// Mesh initialization/destruction.
void null_mesh(mesh_t *mesh);
int initialize_mesh(mesh_t *mesh, int num_vertices);
void destroy_mesh(mesh_t *mesh);

#endif // _MESH_H_