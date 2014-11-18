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
	Renderer::IndexBuffer *indexBuffer = resources->CreateIndexBuffer();
	if (indexBuffer == nullptr) {
		ErrorStack::Log("Failed to create index buffer for segment.");
		return false;
	}
	this->indexBuffer = indexBuffer;

	// Load the data into the buffer.
	indexBuffer->Load(indices, arraySize, indexCount, Renderer::IntType);
	return true;
}

// Model-generic buffer schema.
Renderer::BufferSchema *EntityModelFrame::vertexSchema = nullptr;

EntityModelFrame::EntityModelFrame() : vertexBuffer(nullptr)
{
}

EntityModelFrame::~EntityModelFrame()
{
}

// Initialize model frame.
bool EntityModelFrame::Initialize(int vertexCount)
{
	// Allocate mesh.
	if (!mesh.Initialize(vertexCount)) {
		ErrorStack::Log("Failed to initialize mesh for frame vertices.");
		return false;
	}
	return true;
}

// Create a renderer reference from the vertex data.
bool EntityModelFrame::LoadRendererResources(Renderer::Resources *resources)
{
	// Pass vertex data to renderer resource loader.
	Renderer::Buffer *vertexBuffer = resources->CreateBuffer(
		mesh.GetVertexBuffer(),
		mesh.GetVertexBufferSize());
	if (vertexBuffer == nullptr) {
		ErrorStack::Log("Failed to create vertex buffer for frame.");
		return false;
	}
	this->vertexBuffer = vertexBuffer;
	return true;
}

// Free vertex buffer for this frame.
void EntityModelFrame::FreeRendererResources(Renderer::Resources *resources)
{
	resources->DestroyBuffer(vertexBuffer);
}

// Copy frame name.
void EntityModelFrame::SetFrameName(const char frameName[FrameNameLength])
{
	// Copy name.
	strncpy(this->frameName, frameName, FrameNameLength);
}

// Set the buffer schema used by all models.
void EntityModelFrame::SetBufferSchema(Renderer::BufferSchema *vertexSchema)
{
	EntityModelFrame::vertexSchema = vertexSchema;
}

// Free the buffer schema used by all models.
void EntityModelFrame::FreeBufferSchema(Renderer::Resources *resources)
{
	resources->DestroyBufferSchema(vertexSchema);
}

EntityModel::EntityModel() : frames(nullptr), frameCount(0)
{
}

EntityModel::~EntityModel()
{
	if (frames != nullptr) {
		MemoryManager::DestroyArray(frames);
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
	}

	// Allocate frame objects and give them their vertices.


	// Allocate segments array.
	if (!MemoryManager::AllocateArray(&segments, segmentCount)) {
		ErrorStack::Log("Failed to allocate ")
		return false;
	}
	return true;
}
