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
			if (!LoadFaces()) {
				return false;
			}
			return true;
		}

		// Loads faces from the lump and saves them into the output map object.
		// Returns true on success, false otherwise.
		bool Parser::LoadFaces()
		{
			// Prepare faces for filling.
			const Lump *facesLump = &header->lumps[FacesLump];
			int32_t faceCount = facesLump->length / sizeof(Face);
			const BSP::FileFormat::Face *inputFace =
				reinterpret_cast<const Face*>(file.GetBuffer() + facesLump->offset);
			if (!out->InitializeFaces(faceCount)) {
				return false;
			}

			// Get the surface edges table, edges array, and vertices.
			const Lump *edgesLump = &header->lumps[EdgesLump];
			const Edge *edges = reinterpret_cast<const Edge*>(file.GetBuffer() + edgesLump->offset);
			const Lump *surfaceEdgesLump = &header->lumps[SurfaceEdgesTableLump];
			const SurfaceEdge *surfaceEdges = 
				reinterpret_cast<const SurfaceEdge*>(file.GetBuffer() + surfaceEdgesLump->offset);
			const Lump *verticesLump = &header->lumps[VerticesLump];
			const Vector3 *vertices = reinterpret_cast<const Vector3*>(file.GetBuffer() + verticesLump->offset);

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
				const SurfaceEdge *currentEdgeEntry = &surfaceEdges[inputFace->firstEdge];
				for (int16_t j = 0; j < edgeCount; ++j, ++currentEdgeEntry, ++outputVertex) {
					int32_t edgeIndex = currentEdgeEntry->edgeIndex;

					// If negative index, it's counter-clockwise order, so get end vertex.
					const Edge *currentEdge;
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

	}

}