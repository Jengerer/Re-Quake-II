#include "bsp_parser.h"
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
		bool Parser::Load(const char *name, BSP::Map *out)
		{
			this->out = out;
			if (!file.Read(name)) {
				ErrorStack::Log("Failed to open map file '%s'.", name);
				return false;
			}
			header = reinterpret_cast<const Header*>(file.GetBuffer());
			if (header->magic != MagicNumber) {
				ErrorStack::Log("Invalid format retrieved, header mismatch.");
				return false;
			}
			if (!PrepareLumps()) {
				return false;
			}

			// Load all map segments.
			if (!LoadPlanes()) {
				return false;
			}
			// Faces need planes, so load after.
			if (!LoadFaces()) {
				return false;
			}
			// Nodes need faces and planes, so load after.
			if (!LoadNodes()) {
				return false;
			}
			return true;
		}

		// Prepare the lump pointers/counts and verify that they're valid.
		bool Parser::PrepareLumps()
		{
			// Go through each lump and validate its size against the element structure.
			for (int32_t i = EntitiesLump; i != LumpCount; ++i) {
				const FileFormat::Lump *lump = &header->lumps[i];

				// Get the validation parameters.
				int32_t elementSize; // The size of each element in the lump.
				const void **lumpReference; // The pointer to fill out with the lump location.
				int32_t *lumpSizeReference; // The integer, if any, to fill out with lump element size.
				switch (i) {
				case PlanesLump:
					elementSize = sizeof(FileFormat::Plane);
					lumpReference = reinterpret_cast<const void**>(&planes);
					lumpSizeReference = &planeCount;
					break;
				case VerticesLump:
					elementSize = sizeof(Vector3);
					lumpReference = reinterpret_cast<const void**>(&vertices);
					lumpSizeReference = nullptr;
					break;
				case NodesLump:
					elementSize = sizeof(FileFormat::Node);
					lumpReference = reinterpret_cast<const void**>(&nodes);
					lumpSizeReference = &nodeCount;
					break;
				case FacesLump:
					elementSize = sizeof(FileFormat::Face);
					lumpReference = reinterpret_cast<const void**>(&faces);
					lumpSizeReference = &faceCount;
					break;
				case EdgesLump:
					elementSize = sizeof(FileFormat::Edge);
					lumpReference = reinterpret_cast<const void**>(&edges);
					lumpSizeReference = nullptr;
					break;
				case SurfaceEdgesTableLump:
					elementSize = sizeof(FileFormat::SurfaceEdge);
					lumpReference = reinterpret_cast<const void**>(&surfaceEdges);
					lumpSizeReference = nullptr;
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
				*lumpReference = file.GetBuffer() + lump->offset;
				if (lumpSizeReference != nullptr) {
					*lumpSizeReference = lumpSize / elementSize;
				}
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
				minimums.x = static_cast<float>(inputNode->minimums.x);
				minimums.y = static_cast<float>(inputNode->minimums.y);
				minimums.z = static_cast<float>(inputNode->minimums.z);
				maximums.x = static_cast<float>(inputNode->maximums.x);
				maximums.y = static_cast<float>(inputNode->maximums.y);
				maximums.z = static_cast<float>(inputNode->maximums.z);
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
			for (int32_t i = 0; i < faceCount; ++i, ++inputFace, ++outputFace) {
				int16_t edgeCount = inputFace->edgeCount;
				if (!outputFace->Initialize(edgeCount)) {
					return false;
				}
				FaceMesh *outputMesh = outputFace->GetMesh();
				FaceVertex *outputVertex = outputMesh->GetVertexBuffer();

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

					// Make Y up instead of Z.
					outputVertex->position.x = currentVertex->x;
					outputVertex->position.y = currentVertex->z;
					outputVertex->position.z = currentVertex->y;
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
				outPlane->normal = inputPlane->normal;
				outPlane->distance = inputPlane->distance;
			}
			return true;
		}

	}

}