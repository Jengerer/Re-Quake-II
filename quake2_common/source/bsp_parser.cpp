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

			// Load all map segments.
			if (!LoadNodes()) {
				return false;
			}
			if (!LoadFaces()) {
				return false;
			}
			if (!LoadPlanes()) {
				return false;
			}
			return true;
		}

		// Load the non-leaf nodes from the file into the map.
		// Returns true on success, false otherwise.
		bool Parser::LoadNodes()
		{
			const FileFormat::Lump *nodesLump = &header->lumps[NodesLump];
			int32_t nodeCount = nodesLump->length / sizeof(FileFormat::Lump);
			const FileFormat::Node *nodes =
				reinterpret_cast<const FileFormat::Node*>(file.GetBuffer() + nodesLump->offset);
			
			return true;
		}

		// Loads faces from the lump and saves them into the output map object.
		// Returns true on success, false otherwise.
		bool Parser::LoadFaces()
		{
			const FileFormat::Lump *facesLump = &header->lumps[FacesLump];
			int32_t faceCount = facesLump->length / sizeof(Face);
			const FileFormat::Face *inputFace =
				reinterpret_cast<const FileFormat::Face*>(file.GetBuffer() + facesLump->offset);
			if (!out->InitializeFaces(faceCount)) {
				return false;
			}

			// Get the surface edges table, edges array, and vertices.
			const FileFormat::Lump *edgesLump = &header->lumps[EdgesLump];
			const FileFormat::Lump *surfaceEdgesLump = &header->lumps[SurfaceEdgesTableLump];
			const FileFormat::Lump *verticesLump = &header->lumps[VerticesLump];
			const FileFormat::Edge *edges = 
				reinterpret_cast<const FileFormat::Edge*>(file.GetBuffer() + edgesLump->offset);
			const FileFormat::SurfaceEdge *surfaceEdges = 
				reinterpret_cast<const FileFormat::SurfaceEdge*>(file.GetBuffer() + surfaceEdgesLump->offset);
			const Vector3 *vertices =
				reinterpret_cast<const Vector3*>(file.GetBuffer() + verticesLump->offset);

			// Go through each face.
			BSP::Face *outputFace = out->GetFaces();
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
			const FileFormat::Lump *planesLump = &header->lumps[PlanesLump];
			int16_t planeCount = static_cast<int16_t>(planesLump->length / sizeof(FileFormat::Plane));
			if (!out->InitializePlanes(planeCount)) {
				return false;
			}

			Geometry::Plane *outPlane = out->GetPlanes();
			const FileFormat::Plane *inputPlane =
				reinterpret_cast<const FileFormat::Plane*>(file.GetBuffer() + planesLump->offset);
			for (int16_t i = 0; i < planeCount; ++i, ++inputPlane, ++outPlane) {
				outPlane->normal = inputPlane->normal;
				outPlane->distance = inputPlane->distance;
			}
			return true;
		}

	}

}