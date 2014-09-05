#ifndef _BSP_MAP_H_
#define _BSP_MAP_H_

#include "vector3d.h"
#include <stdint.h>

// Structure for 3D short vector.
typedef struct vector3d_short
{
	int16_t x;
	int16_t y;
	int16_t z;
} vector3d_short_t;

// BSP plane structure.
typedef struct bsp_plane
{
	vector3d_t normal;
	float distance;
	uint32_t type;
} bsp_plane_t;

// BSP tree node structure.
typedef struct bsp_node
{
	// Splitting plane index.
	uint32_t plane;

	// Children indices.
	int32_t child_front;
	int32_t child_back;

	// Bounding box min and max points.
	vector3d_short_t bbox_min;
	vector3d_short_t bbox_max;

	// Faces index/length that are on this plane.
	uint16_t face_start;
	uint16_t face_count;
};

// BSP map structure.
typedef struct bsp_map
{
	// Splitting plane array.
	bsp_plane_t *planes;
	int num_planes;
} bsp_map_t;

// Map initialization and destruction.
void bsp_map_null(bsp_map_t *map);
void bsp_map_destroy(bsp_map_t *map);

// Splitting planes functions.
void bsp_map_null_planes(bsp_map_t *map);
int bsp_map_initialize_planes(bsp_map_t *map, int num_planes);
void bsp_map_destroy_planes(bsp_map_t *map);

#endif // _BSP_MAP_H_