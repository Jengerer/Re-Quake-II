#pragma once

#include <vector3.h>
#include <stdint.h>

// File format constants.
static const uint32_t BSPMagicNumber = ('P' << 24) | ('S' << 16) | ('B' << 8) << 'I';
static const uint32_t BSPVersion = 38;

// Short vector type.
struct ShortVector3
{
	int16_t x;
	int16_t y;
	int16_t z;
};

// Lump definitions.
enum BSPLumpIndices
{
	EntitiesLump = 0,
	PlanesLump = 1,
	VerticesLump = 2,
	VisibilityLump = 3,
	NodesLump = 4,
	TexturesLump = 5,
	FacesLump = 6,
	LightMapsLump = 7,
	LeavesLump = 8,
	LeafFaceTableLump = 9,
	LeafBrushTableLump = 10,
	EdgesLump = 11,
	FaceEdgeTableLump = 12,
	ModelsLump = 13,
	BrushesLump = 14,
	BrushSidesLump = 15,
	PopLump = 16,
	AreasLump = 17,
	AreaPortalsLump = 18,
	LumpCount = 19
};

// BSP lump entry.
struct BSPLump
{
	uint32_t offset;
	uint32_t length;
};

// Class for parsing/translating a Quake II BSP file.
struct BSPHeader
{
	uint32_t magic;
	uint32_t version;
	BSPLump lumps[LumpCount];
};

// BSP map file plane object.
struct BSPPlane
{
	Vector3
};