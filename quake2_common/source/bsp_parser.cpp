#include "bsp_parser.h"
#include "quake_file_manager.h"
#include <error_stack.h>

namespace BSP
{

	namespace FileFormat
	{

		Parser::Parser() : out(nullptr)
		{
		}

		Parser::~Parser()
		{
		}

		// Load the map and fill out the output map.
		bool Parser::Load(const char *filename, BSP::Map *out)
		{
			// Get file data.
			FileData mapData;
			QuakeFileManager *quakeFiles = QuakeFileManager::GetInstance();
			if (!quakeFiles->Read(filename, &mapData)) {
				return false;
			}
			this->data = mapData.GetData();
			this->out = out;
			header = reinterpret_cast<const Header*>(data);
			if (header->magic != MagicNumber) {
				ErrorStack::Log("Bad map format, mismatched header.");
				return false;
			}
			if (header->version != Version) {
				ErrorStack::Log("Bad map version, expected %d and read %d.", Version, header->version);
				return false;
			}
			if (!PrepareLumps()) {
				return false;
			}

			// Load all map segments.
			// Faces and nodes need planes loaded.
			if (!LoadPlanes()) {
				return false;
			}
			// Faces require face textures.
			if (!LoadTextures()) {
				return false;
			}
			// Nodes and leaf face table need faces loaded.
			if (!LoadFaces()) {
				return false;
			}
			if (!LoadNodes()) {
				return false;
			}
			// Brushes need sides loaded.
			if (!LoadBrushSides()) {
				return false;
			}
			// Leaves and leaf brush table need brushes loaded.
			if (!LoadBrushes()) {
				return false;
			}
			// Leaves need visibility clusters loaded.
			if (!LoadVisibility()) {
				return false;
			}
			// Leaves need leaf face table.
			if (!LoadLeafFaces()) {
				return false;
			}
			// Leaves need leaf brush table.
			if (!LoadLeafBrushes()) {
				return false;
			}
			if (!LoadLeaves()) {
				return false;
			}

			// Build leaf/node parent graph.
			out->BuildParentGraph();
			return true;
		}

		// Prepare the lump pointers/counts and verify that they're valid.
		bool Parser::PrepareLumps()
		{
			// Go through each lump and validate its size against the element structure.
			const FileFormat::Lump *lump;
			for (int32_t i = EntitiesLump; i != LumpCount; ++i) {
				lump = &header->lumps[i];

				// Get the validation parameters.
				int32_t elementSize; // The size of each element in the lump.
				const void **lumpReference; // The pointer to fill out with the lump location.
				int32_t *lumpElementCount; // The integer, if any, to fill out with lump element size.
				switch (i) {
				case PlanesLump:
					elementSize = sizeof(FileFormat::Plane);
					lumpReference = reinterpret_cast<const void**>(&planes);
					lumpElementCount = &planeCount;
					break;
				case VerticesLump:
					elementSize = sizeof(Vector3);
					lumpReference = reinterpret_cast<const void**>(&vertices);
					lumpElementCount = nullptr;
					break;
				case VisibilityLump:
					// Visibility isn't just an array of elements; requires lump size.
					elementSize = sizeof(uint8_t);
					lumpReference = reinterpret_cast<const void**>(&visibilityStart);
					lumpElementCount = &visibilityLength;
					break;
				case NodesLump:
					elementSize = sizeof(FileFormat::Node);
					lumpReference = reinterpret_cast<const void**>(&nodes);
					lumpElementCount = &nodeCount;
					break;
				case TexturesLump:
					elementSize = sizeof(FileFormat::Texture);
					lumpReference = reinterpret_cast<const void**>(&textures);
					lumpElementCount = &textureCount;
					break;
				case FacesLump:
					elementSize = sizeof(FileFormat::Face);
					lumpReference = reinterpret_cast<const void**>(&faces);
					lumpElementCount = &faceCount;
					break;
				case LeavesLump:
					elementSize = sizeof(FileFormat::Leaf);
					lumpReference = reinterpret_cast<const void**>(&leaves);
					lumpElementCount = &leafCount;
					break;
				case LeafFacesLump:
					elementSize = sizeof(int16_t);
					lumpReference = reinterpret_cast<const void**>(&leafFaces);
					lumpElementCount = &leafFaceCount;
					break;
				case LeafBrushesLump:
					elementSize = sizeof(uint16_t);
					lumpReference = reinterpret_cast<const void**>(&leafBrushes);
					lumpElementCount = &leafBrushCount;
					break;
				case EdgesLump:
					elementSize = sizeof(FileFormat::Edge);
					lumpReference = reinterpret_cast<const void**>(&edges);
					lumpElementCount = nullptr;
					break;
				case SurfaceEdgesLump:
					elementSize = sizeof(FileFormat::SurfaceEdge);
					lumpReference = reinterpret_cast<const void**>(&surfaceEdges);
					lumpElementCount = nullptr;
					break;
				case BrushesLump:
					elementSize = sizeof(FileFormat::Brush);
					lumpReference = reinterpret_cast<const void**>(&brushes);
					lumpElementCount = &brushCount;
					break;
				case BrushSidesLump:
					elementSize = sizeof(FileFormat::BrushSide);
					lumpReference = reinterpret_cast<const void**>(&brushSides);
					lumpElementCount = &brushSideCount;
					break;
				default:
					continue;
				}

				// Verify the lump based on parameters.
				int32_t lumpSize = lump->length;
				if ((lumpSize % elementSize) != 0) {
					ErrorStack::Log("Bad map format: element size mismatch for lump %d.", i);
					return false;
				}
				*lumpReference = data + lump->offset;
				if (lumpElementCount != nullptr) {
					*lumpElementCount = lumpSize / elementSize;
				}
			}
			return true;
		}

		// Parse and copy the separating planes into the map.
		bool Parser::LoadPlanes()
		{
			int32_t planeCount = this->planeCount;
			if (!out->InitializePlanes(planeCount)) {
				return false;
			}

			Geometry::Plane *outPlane = out->GetPlanes();
			const FileFormat::Plane *inputPlane = planes;
			for (int16_t i = 0; i < planeCount; ++i, ++inputPlane, ++outPlane) {
				outPlane->normal.FromQuakeCoordinates(
					inputPlane->normal.x,
					inputPlane->normal.y,
					inputPlane->normal.z);
				outPlane->distance = inputPlane->distance;
			}
			return true;
		}

		// Loads textures from the lump into the output map.
		// Returns true on success, false otherwise.
		bool Parser::LoadTextures()
		{
			int32_t textureCount = this->textureCount;
			if (!out->InitializeTextures(textureCount)) {
				return false;
			}
			BSP::FaceTexture *outputTexture = out->GetTextures();
			const FileFormat::Texture *inputTexture = textures;
			for (int32_t i = 0; i < textureCount; ++i, ++inputTexture, ++outputTexture) {
				outputTexture->SetName(inputTexture->name);
			}
			return true;
		}

		// Loads faces from the lump and saves them into the output map object.
		// Returns true on success, false otherwise.
		bool Parser::LoadFaces()
		{
			int32_t faceCount = this->faceCount;
			if (!out->InitializeFaces(faceCount)) {
				return false;
			}

			// Get the surface edges table, edges array, and vertices.
			BSP::Face *outputFace = out->GetFaces();
			const FileFormat::Face *inputFace = faces;
			const FileFormat::Texture *fileTextures = textures;
			const BSP::FaceTexture *mapTextures = out->GetTextures();
			for (int32_t i = 0; i < faceCount; ++i, ++inputFace, ++outputFace) {
				int16_t edgeCount = inputFace->edgeCount;
				if (!outputFace->Initialize(edgeCount)) {
					return false;
				}
				FaceMesh *outputMesh = outputFace->GetMesh();
				FaceVertex *outputVertex = outputMesh->GetVertexBuffer();

				// Get the texture information structure to get UV coordinates.
				int16_t textureIndex = inputFace->textureIndex;
				const FileFormat::Texture *currentTexture = &fileTextures[textureIndex];

				// Build polygon from face edges.
				const FileFormat::SurfaceEdge *currentEdgeEntry = &surfaceEdges[inputFace->firstEdge];
				for (int16_t j = 0; j < edgeCount; ++j, ++currentEdgeEntry, ++outputVertex) {
					int32_t edgeIndex = currentEdgeEntry->edgeIndex;

					// If negative index, it's counter-clockwise order, so get end vertex.
					const FileFormat::Edge *currentEdge;
					const Vector3 *currentVertex;
					if (edgeIndex < 0) {
						currentEdge = &edges[-edgeIndex];
						currentVertex = &vertices[currentEdge->endIndex];
					}
					else {
						currentEdge = &edges[edgeIndex];
						currentVertex = &vertices[currentEdge->startIndex];
					}
					outputVertex->position.FromQuakeCoordinates(
						currentVertex->x,
						currentVertex->y,
						currentVertex->z);

					// Calculate U and V.
					outputVertex->uv.x = (currentVertex->DotProduct(currentTexture->scaleS) + currentTexture->offsetS);
					outputVertex->uv.y = currentVertex->DotProduct(currentTexture->scaleT) + currentTexture->offsetT;
					
					// TODO: These are dummy values.
					outputVertex->lightMap.x = 0.f;
					outputVertex->lightMap.y = 0.f;
				}
				
				// Assign the texture from the texture table.
				const BSP::FaceTexture *faceTexture = &mapTextures[textureIndex];
				outputFace->SetTexture(faceTexture);
			}
			return true;
		}

		// Load the non-leaf nodes from the file into the map.
		// Returns true on success, false otherwise.
		bool Parser::LoadNodes()
		{
			int32_t nodeCount = this->nodeCount;
			if (!out->InitializeNodes(nodeCount)) {
				return false;
			}

			// Convert raw node to map node.
			const Geometry::Plane *planes = out->GetPlanes();
			const BSP::Face *faces = out->GetFaces();
			const FileFormat::Node *inputNode = nodes;
			BSP::Node *outNode = out->GetNodes();
			for (int32_t i = 0; i < nodeCount; ++i, ++inputNode, ++outNode) {
				// Cast bounds to float vector.
				Vector3 minimums;
				Vector3 maximums;
				minimums.FromQuakeCoordinates(
					static_cast<float>(inputNode->minimums.x),
					static_cast<float>(inputNode->minimums.y),
					static_cast<float>(inputNode->minimums.z));
				maximums.FromQuakeCoordinates(
					static_cast<float>(inputNode->maximums.x),
					static_cast<float>(inputNode->maximums.y),
					static_cast<float>(inputNode->maximums.z));

				outNode->SetParameters(
					&planes[inputNode->planeIndex],
					inputNode->frontChild,
					inputNode->backChild,
					minimums,
					maximums,
					&faces[inputNode->firstFace],
					inputNode->faceCount);
			}
			return true;
		}

		// Parse and copy brush sides.
		bool Parser::LoadBrushSides()
		{
			int32_t brushSideCount = this->brushSideCount;
			if (!out->InitializeBrushSides(brushSideCount)) {
				return false;
			}

			const Geometry::Plane *mapPlanes = out->GetPlanes();
			BSP::BrushSide *outputSide = out->GetBrushSides();
			const FileFormat::BrushSide *inputSide = brushSides;
			for (int32_t i = 0; i < brushSideCount; ++i, ++inputSide, ++outputSide) {
				outputSide->SetParameters(
					&mapPlanes[inputSide->planeIndex],
					inputSide->textureIndex);
			}
			return true;
		}

		// Parse and copy brushes to map.
		bool Parser::LoadBrushes()
		{
			int32_t brushCount = this->brushCount;
			if (!out->InitializeBrushes(brushCount)) {
				return false;
			}
		
			const BSP::BrushSide *mapSides = out->GetBrushSides();
			BSP::Brush *outputBrush = out->GetBrushes();
			const FileFormat::Brush *inputBrush = brushes;
			for (int32_t i = 0; i < brushCount; ++i) {
				outputBrush->SetParameters(
					&mapSides[inputBrush->firstSide],
					inputBrush->sideCount,
					inputBrush->contents);
			}
			return true;
		}

		// Parse and copy visibility data.
		bool Parser::LoadVisibility()
		{
			const FileFormat::VisibilityHeader *header =
				reinterpret_cast<const FileFormat::VisibilityHeader*>(visibilityStart);
			int32_t clusterCount = header->clusterCount;

			// Get size of just visibility data (excluding headers/clusters).
			int32_t dataStart = sizeof(FileFormat::VisibilityHeader) + 
				(clusterCount * sizeof(FileFormat::VisibilityCluster));
			int32_t dataSize = visibilityLength - dataStart;
			if (!out->InitializeClusters(clusterCount, dataSize)) {
				return false;
			}

			// Copy the raw data over.
			uint8_t *mapData = out->GetClusterData();
			uint8_t *outData = mapData;
			const uint8_t *inputData = visibilityStart + dataStart;
			for (int32_t i = 0; i < dataSize; ++i, ++inputData, ++outData) {
				*outData = *inputData;
			}

			// Clusters immediately follow header in file.
			BSP::LeafCluster *outCluster = out->GetClusters();
			const FileFormat::VisibilityCluster *inputCluster =
				reinterpret_cast<const FileFormat::VisibilityCluster*>(header + 1);
			for (int32_t i = 0; i < clusterCount; ++i, ++inputCluster, ++outCluster) {
				// Translate offsets to just cluster data.
				outCluster->SetParameters(
					&mapData[inputCluster->visibilityOffset - dataStart],
					&mapData[inputCluster->audibilityOffset - dataStart]);
			}
			return true;
		}

		// Parse and map the leaf face table.
		bool Parser::LoadLeafFaces()
		{
			int32_t leafFaceCount = this->leafFaceCount;
			if (!out->InitializeLeafFaces(leafFaceCount)) {
				return false;
			}

			BSP::Face *mapFaces = out->GetFaces();
			BSP::Face **outEntry = out->GetLeafFaces();
			const int16_t *inputIndex = leafFaces;
			for (int32_t i = 0; i < leafFaceCount; ++i, ++inputIndex, ++outEntry) {
				// Map the index to the face address.
				int16_t currentIndex = *inputIndex;
				*outEntry = &mapFaces[currentIndex];
			}
			return true;
		}

		// Parse and map the leaf brush table.
		bool Parser::LoadLeafBrushes()
		{
			int32_t leafBrushCount = this->leafBrushCount;
			if (!out->InitializeLeafBrushes(leafBrushCount)) {
				return false;
			}

			BSP::Brush *mapBrushes = out->GetBrushes();
			BSP::Brush **outEntry = out->GetLeafBrushes();
			const uint16_t *inputIndex = leafBrushes;
			for (int32_t i = 0; i < leafBrushCount; ++i, ++inputIndex, ++outEntry) {
				// Map the index to the brush address.
				uint16_t currentIndex = *inputIndex;
				*outEntry = &mapBrushes[currentIndex];
			}
			return true;
		}

		// Parse and copy leaves to map.
		bool Parser::LoadLeaves()
		{
			int32_t leafCount = this->leafCount;
			if (!out->InitializeLeaves(leafCount)) {
				return false;
			}

			BSP::Face **mapLeafFaces = out->GetLeafFaces();
			BSP::Brush **mapLeafBrushes = out->GetLeafBrushes();
			BSP::Leaf *outputLeaf = out->GetLeaves();
			const FileFormat::Leaf *inputLeaf = leaves;
			for (int32_t i = 0; i < leafCount; ++i, ++inputLeaf, ++outputLeaf) {
				// Convert bounds to float vector, swap Y with Z.
				Vector3 minimums;
				Vector3 maximums;
				minimums.FromQuakeCoordinates(
					static_cast<float>(inputLeaf->minimums.x),
					static_cast<float>(inputLeaf->minimums.y),
					static_cast<float>(inputLeaf->minimums.z));
				maximums.FromQuakeCoordinates(
					static_cast<float>(inputLeaf->maximums.x),
					static_cast<float>(inputLeaf->maximums.y),
					static_cast<float>(inputLeaf->maximums.z));

				// Resolve pointers.
				outputLeaf->SetParameters(
					inputLeaf->contents,
					inputLeaf->clusterIndex,
					inputLeaf->areaIndex,
					minimums,
					maximums,
					&mapLeafFaces[inputLeaf->firstFace],
					inputLeaf->faceCount,
					&mapLeafBrushes[inputLeaf->firstBrush],
					inputLeaf->brushCount);
			}
			return true;
		}

	}

}
