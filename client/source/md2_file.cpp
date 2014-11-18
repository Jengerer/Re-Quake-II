#include "error_stack.h"
#include "md2_file.h"

MD2File::MD2File(EntityModel *out)
	: out(out),
	header(nullptr)
{
}

MD2File::~MD2File()
{
}

// Load a model from a file.
bool MD2File::Load(const char *filename)
{
	// Load the file into memory.
	if (!file.Read(filename)) {
		ErrorStack::Log("Failed to load model from file: %s", filename);
		return false;
	}

	// Set up pointers to file segments.
	const char *modelBuffer = file.GetBuffer();
	header = reinterpret_cast<const MD2Header*>(modelBuffer);
	if (!VerifyHeader()) {
		return false;
	}
	commands = reinterpret_cast<const MD2Command*>(file.GetBuffer() + header->commandsOffset);

	// Initialize the model.
	if (!out->Initialize(header->frameCount, header->commandCount, header->vertexCount)) {
		ErrorStack::Log("Failed to initialize model for loading: %s", filename);
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
bool MD2File::VerifyHeader()
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
void MD2File::LoadFrames()
{
	// Keep a reference for the vertices.
	TexturedMesh *outMesh = out->GetMesh();
	TexturedVertex *outVertex = outMesh->GetVertexBuffer();
	int vertexCount = outMesh->GetVertexCount();

	// Get space between frames.
	const int32_t FrameCount = header->frameCount;
	const int32_t FrameStride = sizeof(MD2Frame) + (vertexCount * sizeof(MD2Vertex));
	const int32_t FrameStart = header->framesOffset;
	const int32_t FrameEnd = FrameStart + (FrameCount * FrameStride);

	// Get frames we're filling out.
	EntityModelFrame *outFrame = out->GetFrames();

	// Go through each frame.
	for (int32_t i = FrameStart; i < FrameEnd; i += FrameStride, ++outFrame) {
		// Get vertices for this frame.
		const MD2Frame *frame = reinterpret_cast<const MD2Frame*>(file.GetBuffer() + i);
		const MD2Vertex *vertex = reinterpret_cast<const MD2Vertex*>(frame + 1);

		// Get frame offset and scale.
		Vector3 scale = frame->scale;
		Vector3 offset = frame->offset;

		// Start the frame vertices at current vertex.
		outFrame->SetVertices(outVertex, vertexCount);

		// Copy frame name over.
		outFrame->SetFrameName(frame->name);

		// Copy vertices.
		for (int j = 0; j < FrameCount; ++j, ++vertex, ++outVertex) {
			outVertex->position.x = (static_cast<float>(vertex->x) * scale.x) + offset.x;
			outVertex->position.y = (static_cast<float>(vertex->y) * scale.y) + offset.y;
			outVertex->position.z = (static_cast<float>(vertex->y) * scale.z) + offset.z;
		}
	}
}

// Load segments and their indices.
bool MD2File::LoadCommands()
{
	// Get segments we're fillin gout.
	EntityModelSegment *outSegment = out->GetSegments();
	const int SegmentCount = out->GetSegmentCount();

	// Go through each command segment.
	int32_t vertexCount;
	Renderer::PrimitiveType commandType;
	const MD2Command *currentCommand = commands;

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

		// Allocate indices.
		if (!outSegment->Initialize(vertexCount, commandType)) {
			ErrorStack::Log("Failed to initialize model segment.");
			return false;
		}
		int *outIndex = outSegment->GetIndexData();

		// Get starting packet.
		const MD2CommandPacket *packet = reinterpret_cast<const MD2CommandPacket*>(currentCommand);

		// Skip appropriate number of packets for next command.
		currentCommand += vertexCount * PacketIntegerCount;

		// Copy indices to index buffer.
		for (; vertexCount > 0; --vertexCount, ++packet, ++outIndex) {
			*outIndex = packet->vertexIndex;
		}
	}
	return true;
}
