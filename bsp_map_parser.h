#ifndef _BSP_MAP_PARSER_H_
#define _BSP_MAP_PARSER_H_

#include "bsp_map.h"
#include <stdint.h>

// BSP header information.
#define BSP_HEADER_MAGIC 0x50534249 // IBSP
#define BSP_VERSION 38

// Lump definitions.
#define BSP_LUMP_ENTITIES 0
#define BSP_LUMP_PLANES 1
#define BSP_LUMP_VERTICES 2
#define BSP_LUMP_VISIBILITY 3
#define BSP_LUMP_NODES 4
#define BSP_LUMP_TEXTURES 5
#define BSP_LUMP_FACES 6
#define BSP_LUMP_LIGHTMAPS 7
#define BSP_LUMP_LEAVES 8
#define BSP_LUMP_LEAF_FACE_TABLE 9
#define BSP_LUMP_LEAF_BRUSH_TABLE 10
#define BSP_LUMP_EDGES 11
#define BSP_LUMP_FACE_EDGE_TABLE 12
#define BSP_LUMP_MODELS 13
#define BSP_LUMP_BRUSHES 14
#define BSP_LUMP_BRUSH_SIDES 15
#define BSP_LUMP_POP 16
#define BSP_LUMP_AREAS 17
#define BSP_LUMP_AREA_PORTALS 18
#define BSP_LUMP_COUNT 19

// BSP lump for specific data.
typedef struct bsp_lump
{
	uint32_t offset;
	uint32_t length;
} bsp_lump_t;

// Quake II BSP file header structure.
typedef struct bsp_header
{
	uint32_t magic;
	uint32_t version;
	bsp_lump_t lumps[BSP_LUMP_COUNT];
} bsp_header_t;

// Entry BSP parsing function.
int bsp_map_load(const char *filename, bsp_map_t *out);

#endif // _BSP_MAP_PARSER_H_