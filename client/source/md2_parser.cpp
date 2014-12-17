#include "error_stack.h"
#include "md2_parser.h"
#include "quake_normals.h"

MD2Parser::MD2Parser()
	: out(nullptr),
	header(nullptr)
{
}

MD2Parser::~MD2Parser()
{
}

// Load a model from a file.
bool MD2Parser::Load(const uint8_t *modelData, EntityModel *out)
{
	// Set input data.
	this->data = modelData;

	// Set output file.
	this->out = out;

	// Set up pointers to file segments.
	header = reinterpret_cast<const MD2Header*>(data);
	if (!VerifyHeader()) {
		return false;
	}
	commands = reinterpret_cast<const MD2Command*>(data + header->commandsOffset);

	// Initialize the model.
	if (!out->Initialize(header->frameCount, header->vertexCount)) {
		return false;
	}

	// Load the segments.
	if (!LoadCommands()) {
		return false;
	}

	// Load the frames into the model.
	LoadFrames();
	return true;
}

// Verify header.
bool MD2Parser::VerifyHeader()
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
#include <stdio.h>

// Load the frame data.
void MD2Parser::LoadFrames()
{
	// Keep a reference for the vertices.
	EntityModelMesh *outMesh = out->GetMesh();
	EntityModelVertex *outVertex = outMesh->GetVertexBuffer();
	const int VertexCount = header->vertexCount;
	const int BufferSize = VertexCount * sizeof(EntityModelVertex);

	// Get space between frames.
	const int32_t FrameCount = header->frameCount;
	const int32_t FrameStride = sizeof(MD2Frame) + (VertexCount * sizeof(MD2Vertex));
	const int32_t FrameStart = header->framesOffset;
	const int32_t FrameEnd = FrameStart + (FrameCount * FrameStride);

	// Get frames we're filling out.
	EntityModelFrame *outFrame = out->GetFrames();
	for (int32_t i = FrameStart; i < FrameEnd; i += FrameStride, ++outFrame) {
		const MD2Frame *frame = reinterpret_cast<const MD2Frame*>(data + i);
		const MD2Vertex *vertex = reinterpret_cast<const MD2Vertex*>(frame + 1);
		Vector3 scale = frame->scale;
		Vector3 offset = frame->offset;

		// Start the frame vertices at current vertex.
		outFrame->SetVertices(outVertex, BufferSize);
		outFrame->SetFrameName(frame->name);
		for (int j = 0; j < VertexCount; ++j, ++vertex, ++outVertex) {
			outVertex->position.FromQuakeCoordinates(
				(static_cast<float>(vertex->x) * scale.x) + offset.x,
				(static_cast<float>(vertex->y) * scale.y) + offset.y,
				(static_cast<float>(vertex->z) * scale.z) + offset.z);
			const Vector3 *quakeNormal = &QuakeNormals[vertex->normalIndex];
			outVertex->normal.FromQuakeCoordinates(
				quakeNormal->x,
				quakeNormal->y,
				quakeNormal->z);
		}
	}
}

// Load segments and their indices.
bool MD2Parser::LoadCommands()
{
	// Go through each command segment.
	int32_t vertexCount;
	Renderer::PrimitiveType commandType;
	const MD2Command *currentCommand = commands;

	// First count the number of primitives.
	// When number of vertices to draw is 0, we're done.
	int segmentCount = 0;
	for (currentCommand = commands; (vertexCount = (currentCommand++)->vertexCount) != 0; ++segmentCount) {
		if (vertexCount < 0) {
			vertexCount = -vertexCount;
		}
		currentCommand += (vertexCount * PacketIntegerCount);
	}

	// Don't count the last segment (it's the zero command).
	int indexCount = header->commandCount - (segmentCount + 1);
	if (!out->InitializeSegments(indexCount, segmentCount)) {
		return false;
	}
	unsigned int *outIndex = out->GetIndexData();
	EntityModelSegment *outSegment = out->GetSegments();

	// Go through each command segment.
	// When number of vertices to draw is 0, we're done.
	for (currentCommand = commands; (vertexCount = (currentCommand++)->vertexCount) != 0; ++outSegment) {
		// If it's negative, we're drawing a fan.
		if (vertexCount < 0) {
			commandType = Renderer::TriangleFan;
			vertexCount = -vertexCount;
		}
		else {
			commandType = Renderer::TriangleStrip;
		}
		outSegment->SetParameters(outIndex, vertexCount, commandType);

		// Copy indices to index buffer.
		const MD2CommandPacket *packet = reinterpret_cast<const MD2CommandPacket*>(currentCommand);
		currentCommand += vertexCount * PacketIntegerCount;
		for (; vertexCount > 0; --vertexCount, ++packet, ++outIndex) {
			*outIndex = static_cast<unsigned int>(packet->vertexIndex);
		}
	}
	return true;
}
