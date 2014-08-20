#ifndef _MESH_H_
#define _MESH_H_

#include "vector3d.h"
#include "vector2d.h"

// Struct for representing mesh vertices and texture coordinates.
typedef struct mesh_vertex
{
	vector3d_t position;
	vector2d_t texture;
} mesh_vertex_t;

// Generic mesh of vertices.
typedef struct mesh
{
	// Vertices and indices to them.
	mesh_vertex_t *vertices;
	int num_vertices;
} mesh_t;

// Mesh initialization/destruction.
void mesh_null(mesh_t *mesh);
int mesh_initialize(mesh_t *mesh, int num_vertices);
void mesh_destroy(mesh_t *mesh);

#endif // _MESH_H_