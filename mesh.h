#ifndef _MESH_H_
#define _MESH_H_

#include "vector3d.h"
#include "Vector3d.h"

// Struct for representing mesh vertices and texture coordinates.
typedef struct mesh_vertex
{
	vector3d_t position;
	Vector3d_t texture;
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

// Mesh vertex functions.
int mesh_get_num_vertices(const mesh_t *mesh);
mesh_vertex_t* mesh_get_vertex(mesh_t *mesh, int index);

#endif // _MESH_H_