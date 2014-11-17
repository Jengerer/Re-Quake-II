#pragma once

#include "bsp_map.h"
#include <stdint.h>

// BSP header information.
#define BSP_HEADER_MAGIC 0x50534249 // IBSP
#define BSP_VERSION 38

// Lump definitions.
enum BSPLumpIndices
{
	LumpEntities = 0,
	LumpPlanes = 1,
	LumpVertices = 2,
	LumpVisibility = 3,
	LumpNodes = 4,
	LumpTextures = 5,
	LumpFaces = 6,
	LumpLightmaps = 7,
	LumpLeaves = 8,
	LumpLeafFaceTable = 9,
	LumpLeafBrushTable = 10,
	LumpEdges = 11,
	LumpFaceEdgeTable = 12,
	LumpModels = 13,
	LumpBrushes = 14,
	LumpBrushSides = 15,
	LumpPop = 16,
	LumpAreas = 17,
	LumpAreaPortals = 18,
	LumpCount = 19
};

// BSP lump for specific data.
struct BSPLump
{
	uint32_t offset;
	uint32_t length;
};

// Quake II BSP file header structure.
struct BSPHeader
{
	uint32_t magic;
	uint32_t version;
	BSPLump lumps[LumpCount];
};