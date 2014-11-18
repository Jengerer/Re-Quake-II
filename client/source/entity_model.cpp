#include "entity_model.h"
#include "error_stack.h"
#include "memory_manager.h"
#include <string.h>

EntityModelSegment::EntityModelSegment()
	: indices(nullptr),
	indexBuffer(nullptr)
{
}

EntityModelSegment::~EntityModelSegment()
{
	// Free renderer resource.
	if (indexBuffer != nullptr) {
		indexBuffer->Destroy();
	}

	// Free index buffer.
	if (indices != nullptr) {
		MemoryManager::Free(indices);
	}
}

// Initialize segment for index count.
bool EntityModelSegment::Initialize(int indexCount, Renderer::PrimitiveType type)
{
	// Allocate space for buffer.
	unsigned int arraySize = sizeof(int) * indexCount;
	indices = reinterpret_cast<int*>(MemoryManager::Allocate(arraySize));
	if (indices == nullptr) {
		return false;
	}
	this->arraySize = arraySize;
	this->indexCount = indexCount;
	this->type = type;
	return true;
}

// Prepare the index buffer resource for this segment.
bool EntityModelSegment::LoadResources(Renderer::Resources *resources)
{
	// Create an index buffer with this data.
	int bufferSize = sizeof(int) * indexCount;
	Renderer::IndexBuffer *indexBuffer = resources->CreateIndexBuffer(indices, bufferSize, Renderer::IntType);
	if (indexBuffer == nullptr) {
		ErrorStack::Log("Failed to create index buffer for segment.");
		return false;
	}
	this->indexBuffer = indexBuffer;
	return true;
}

EntityModelFrame::EntityModelFrame() : vertices(nullptr), bufferSize(0), vertexBuffer(nullptr)
{
}

EntityModelFrame::~EntityModelFrame()
{
	// Destroy renderer buffer object.
	if (vertexBuffer != nullptr) {
		vertexBuffer->Destroy();
	}
}

// Set the vertex array for this frame.
void EntityModelFrame::SetVertices(const TexturedVertex *vertices, int bufferSize)
{
	this->vertices = vertices;
	this->bufferSize = bufferSize;
}

// Create a renderer reference from the vertex data.
bool EntityModelFrame::LoadResources(Renderer::Resources *resources)
{
	// Pass vertex data to renderer resource loader.
	Renderer::Buffer *vertexBuffer = resources->CreateBuffer(vertices, bufferSize);
	if (vertexBuffer == nullptr) {
		ErrorStack::Log("Failed to create vertex buffer for frame.");
		return false;
	}
	this->vertexBuffer = vertexBuffer;
	return true;
}

// Copy frame name.
void EntityModelFrame::SetFrameName(const char frameName[FrameNameLength])
{
	// Copy name.
	strncpy(this->frameName, frameName, FrameNameLength);
}

EntityModel::EntityModel() : frames(nullptr), frameCount(0), segments(nullptr), segmentCount(0)
{
}

EntityModel::~EntityModel()
{
	// Delete segment objects.
	if (segments != nullptr) {
		delete[] segments;
	}

	// Delete frame objects.
	if (frames != nullptr) {
		delete[] frames;
	}
}

// Initialize entity model.
bool EntityModel::Initialize(
	int frameCount,
	int segmentCount,
	int vertexCount)
{
	// Allocate enough vertices for all frames.
	const int TotalVertices = frameCount * vertexCount;
	if (!vertices.Initialize(TotalVertices)) {
		ErrorStack::Log("Failed to allocate %d vertices for model.", TotalVertices);
		return false;
	}

	// Allocate frame objects.
	frames = new EntityModelFrame[frameCount];
	if (frames == nullptr) {
		ErrorStack::Log("Failed to allocate %d frame objects for model.", frameCount);
		return false;
	}

	// Allocate segments array.
	segments = new EntityModelSegment[segmentCount];
	if (segments == nullptr) {
		ErrorStack::Log("Failed to allocate %d segment objects for model.", segmentCount);
		return false;
	}
	return true;
}
