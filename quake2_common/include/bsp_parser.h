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
			LeafFacesLump = 9,
			LeafBrushesLump = 10,
			EdgesLump = 11,
			SurfaceEdgesLump = 12,
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

		// Visibility header.
		struct VisibilityHeader
		{
			int32_t clusterCount;
		};

		// Visibility cluster entry.
		struct VisibilityCluster
		{
			int32_t visibilityOffset;
			int32_t audibilityOffset;
		};

		// BSP tree node structure.
		struct Node
		{
			int32_t planeIndex;
			int32_t frontChild;
			int32_t backChild;
			ShortVector3 minimums;
			ShortVector3 maximums;
			uint16_t firstFace;
			uint16_t faceCount;
		};

		// Map texture info structure.
		static const int TextureNameLength = 32;
		struct Texture
		{
			Vector3 scaleS;
			float offsetS;
			Vector3 scaleT;
			float offsetT;
			int32_t flags;
			int32_t value;
			char name[TextureNameLength];
			int32_t nextTextureIndex; // -1 marks end of chain.
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
			int16_t clusterIndex; // Cluster of visibility/audibility sets.
			int16_t areaIndex;
			ShortVector3 minimums;
			ShortVector3 maximums;
			uint16_t firstFace; // Index into leaf faces table, not faces.
			uint16_t faceCount;
			uint16_t firstBrush; // Index into leaf brush table, not brushes.
			uint16_t brushCount;
		};

		// BSP brush structure.
		struct Brush
		{
			int32_t firstSide;
			int32_t sideCount;
			int32_t contents;
		};

		// BSP side structure.
		struct BrushSide
		{
			uint16_t planeIndex;
			int16_t textureIndex;
		};

		// Object for parsing BSP.
		class Quake2CommonLibrary Parser
		{

		public:

			Parser();
			~Parser();

			// Load and fill a map.
			bool Load(const char *filename, BSP::Map *out);

		private:

			// Get the addresses to each lump and verify them.
			bool PrepareLumps();

			// Load each segment into the map.
			bool LoadPlanes();
			bool LoadTextures();
			bool LoadFaces();
			bool LoadNodes();
			bool LoadBrushSides();
			bool LoadBrushes();
			bool LoadVisibility();
			bool LoadLeafFaces();
			bool LoadLeafBrushes();
			bool LoadLeaves();

		private:

			// Buffer data for the map.
			const uint8_t *data;

			// Map to be filled out.
			BSP::Map *out;

			// File header helper.
			const Header *header;

			// Lump pointers and counts.
			const FileFormat::Plane *planes;
			int32_t planeCount;
			const Vector3 *vertices;
			const uint8_t *visibilityStart;
			int32_t visibilityLength;
			const FileFormat::Texture *textures;
			int32_t textureCount;
			const FileFormat::Face *faces;
			int32_t faceCount;
			const FileFormat::Node *nodes;
			int32_t nodeCount;
			const FileFormat::Edge *edges;
			const FileFormat::SurfaceEdge *surfaceEdges;
			const FileFormat::Brush *brushes;
			int32_t brushCount;
			const FileFormat::BrushSide *brushSides;
			int32_t brushSideCount;
			const int16_t *leafFaces;
			int32_t leafFaceCount;
			const uint16_t *leafBrushes;
			int32_t leafBrushCount;
			const FileFormat::Leaf *leaves;
			int32_t leafCount;

		};

	}

}