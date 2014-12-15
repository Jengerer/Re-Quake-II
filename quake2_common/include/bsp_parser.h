#pragma once

#include "bsp_map.h"
#include "quake2_common_define.h"
#include <file.h>
#include <vector3.h>
#include <stdint.h>

namespace BSP
{

	namespace FileFormat
	{

		// File format constants.
		static const uint32_t MagicNumber = ('P' << 24) | ('S' << 16) | ('B' << 8) | 'I';
		static const uint32_t Version = 38;
		static const uint32_t NodeChildrenCount = 2;
		static const uint32_t LightMapStyleCount = 4;

		// Short vector type.
		struct ShortVector3
		{
			int16_t x;
			int16_t y;
			int16_t z;
		};

		// Lump definitions.
		enum LumpIndices
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
			LeafFacesTableLump = 9,
			LeafBrushesTableLump = 10,
			EdgesLump = 11,
			SurfaceEdgesTableLump = 12,
			ModelsLump = 13,
			BrushesLump = 14,
			BrushSidesLump = 15,
			PopLump = 16,
			AreasLump = 17,
			AreaPortalsLump = 18,
			LumpCount = 19
		};

		// BSP lump entry.
		struct Lump
		{
			int32_t offset;
			int32_t length;
		};

		// Class for parsing/translating a Quake II BSP file.
		struct Header
		{
			uint32_t magic;
			uint32_t version;
			Lump lumps[LumpCount];
		};

		// BSP separating plane structure.
		struct Plane
		{
			Vector3 normal;
			float distance;
			int32_t type; // Which axis is normal aligned to, if any.
		};

		// BSP tree node structure.
		struct Node
		{
			int32_t planeIndex;
			int32_t children[NodeChildrenCount];
			ShortVector3 minimums;
			ShortVector3 maximums;
			uint16_t firstFace;
			uint16_t faceCount;
		};

		// BSP leaf face structure.
		struct Face
		{
			uint16_t planeIndex;
			int16_t planeType; // Which axis is normal aligned to, if any.
			int32_t firstEdge;
			int16_t edgeCount;
			int16_t textureIndex;
			int8_t lightStyles[LightMapStyleCount];
			int32_t lightStylesOffset;
		};

		// BSP face edge structure with indices to vertex array.
		struct Edge
		{
			int16_t startIndex;
			int16_t endIndex;
		};

		// BSP surface face edge table entry.
		struct SurfaceEdge
		{
			int32_t edgeIndex;
		};

		// BSP map file leaf object.
		struct Leaf
		{
			int32_t contents; // Content of all brushes in this leaf.
			int16_t visibilityCluster;
			int16_t areaIndex;
			ShortVector3 minimums;
			ShortVector3 maximums;
			uint16_t firstFace;
			uint16_t faceCount;
			uint16_t firstBrush;
			uint16_t brushCount;
		};

		// Object for parsing BSP.
		class Quake2CommonLibrary Parser
		{

		public:

			Parser();
			~Parser();

			// Load and fill a map.
			bool Load(const char *name, BSP::Map *out);

		private:

			bool LoadNodes();
			bool LoadFaces();
			bool LoadPlanes();

		private:

			File file; // Input map file.
			BSP::Map *out; // Map to be filled out.

			// File parsing helpers.
			const Header *header;

		};

	}

}