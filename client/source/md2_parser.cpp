#include "error_stack.h"
#include "md2_parser.h"
#include "quake_normals.h"

namespace MD2
{

	Parser::Parser()
		: out(nullptr),
		header(nullptr)
	{
	}

	Parser::~Parser()
	{
	}

	// Load a model from a file.
	bool Parser::Load(const uint8_t *modelData, EntityModel *out)
	{
		// Set input data.
		this->data = modelData;

		// Set output file.
		this->out = out;

		// Set up pointers to file segments.
		header = reinterpret_cast<const Header*>(data);
		if (!VerifyHeader()) {
			return false;
		}

		// Initialize the model.
		if (!out->Initialize(header->frameCount, header->triangleCount)) {
			return false;
		}

		// Set up triangle references for loading.
		triangles = reinterpret_cast<const Triangle*>(data + header->trianglesOffset);
		triangleCount = header->triangleCount;

		// Load the frames into the model.
		LoadFrames();
		LoadTextureCoordinates();
		return true;
	}

	// Verify header.
	bool Parser::VerifyHeader()
	{
		if (header->magicNumber != MagicNumber) {
			ErrorStack::Log("Bad format: mismatch on header magic number (%x != %x).", header->magicNumber, MagicNumber);
			return false;
		}
		if (header->version != Version) {
			ErrorStack::Log("Bad version number found in map (%d, expected %d).", header->version, Version);
			return false;
		}
		return true;
	}

	// Load the frame data.
	void Parser::LoadFrames()
	{
		// Keep a reference for the vertices.
		EntityModelMesh *outMesh = out->GetMesh();
		EntityModelVertex *outVertex = outMesh->GetVertexBuffer();
		const int VertexCount = header->vertexCount;

		// Prepare for looping through triangles.
		const Triangle *triangles = this->triangles;
		const int32_t TriangleCount = this->triangleCount;
		const int BufferSize = TriangleCount * VerticesPerTriangle * sizeof(EntityModelVertex);

		// Get space between frames.
		const int32_t FrameCount = header->frameCount;
		const int32_t FrameStride = sizeof(Frame) + (VertexCount * sizeof(Vertex));
		const int32_t FrameStart = header->framesOffset;
		const int32_t FrameEnd = FrameStart + (FrameCount * FrameStride);

		// Get frames we're filling out.
		EntityModelFrame *outFrame = out->GetFrames();
		for (int32_t i = FrameStart; i < FrameEnd; i += FrameStride, ++outFrame) {
			const Frame *frame = reinterpret_cast<const Frame*>(data + i);
			const Vertex *frameVertices = reinterpret_cast<const Vertex*>(frame + 1);
			const Vector3 FrameScale = frame->scale;
			const Vector3 FrameOffset = frame->offset;

			// Start the frame vertices at current vertex.
			outFrame->SetVertices(outVertex, BufferSize);
			outFrame->SetFrameName(frame->name);

			// Fill out vertices for each triangle.
			const Triangle *currentTriangle = triangles;
			for (int32_t j = 0; j < TriangleCount; ++j, ++currentTriangle) {
				for (int32_t k = 0; k < VerticesPerTriangle; ++k, ++outVertex) {
					const int32_t VertexIndex = currentTriangle->vertexIndices[k];
					const Vertex *vertex = &frameVertices[VertexIndex];
					outVertex->position.FromQuakeCoordinates(
						(static_cast<float>(vertex->x) * FrameScale.x) + FrameOffset.x,
						(static_cast<float>(vertex->y) * FrameScale.y) + FrameOffset.y,
						(static_cast<float>(vertex->z) * FrameScale.z) + FrameOffset.z);
					const Vector3 *quakeNormal = &QuakeNormals[vertex->normalIndex];
					outVertex->normal.FromQuakeCoordinates(
						quakeNormal->x,
						quakeNormal->y,
						quakeNormal->z);
				}
			}
		}
	}

	// Load the texture coordinate data.
	void Parser::LoadTextureCoordinates()
	{
		// Get reference to output texture coordinates.
		EntityModelTextureCoordinates *outTextureCoordinates = out->GetTextureCoordinates();
		Vector2 *outVector = outTextureCoordinates->GetVertexBuffer();

		// Prepare for copying data.
		const TextureCoordinate *textureCoordinates = reinterpret_cast<const TextureCoordinate*>(data + header->textureCoordinatesOffset);
		const float WidthFactor = 1.f / static_cast<float>(header->skinWidth);
		const float HeightFactor = 1.f / static_cast<float>(header->skinHeight);

		// Go through each triangle and load texture coordinates.
		const Triangle *currentTriangle = triangles;
		const int32_t TriangleCount = this->triangleCount;
		for (int32_t i = 0; i < TriangleCount; ++i, ++currentTriangle) {
			for (int32_t j = 0; j < VerticesPerTriangle; ++j, ++outVector) {
				const int32_t TextureCoordinateIndex = currentTriangle->textureCoordinateIndices[j];
				const TextureCoordinate *inputCoordinate = &textureCoordinates[TextureCoordinateIndex];
				outVector->x = static_cast<float>(inputCoordinate->s) * WidthFactor;
				outVector->y = static_cast<float>(inputCoordinate->t) * HeightFactor;
			}
		}
	}

}
