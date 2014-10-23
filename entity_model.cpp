#include "entity_model.h"
#include "error_stack.h"
#include "memory_manager.h"
#include <string.h>

EntityModelSegment::EntityModelSegment(Renderer::GeometryType geometryType)
	: indices(nullptr),
	indexBuffer(nullptr),
	type(geometryType)
{
}

// Initialize segment for index count.
bool EntityModelSegment::Initialize(int indexCount)
{
	// Allocate space for buffer.
	if (!MemoryManager::AllocateArray(&indices, indexCount)) {
		return false;
	}
	this->indexCount = indexCount;
	return true;
}

// Prepare the index buffer resource for this segment.
bool EntityModelSegment::LoadRendererResources(Renderer::Resources *resources)
{
	// Create an index buffer with this data.
	int bufferSize = sizeof(int) * indexCount;
	Renderer::IndexBuffer *indexBuffer = resources->CreateIndexBuffer(indices, bufferSize, indexCount);
	if (indexBuffer == nullptr) {
		ErrorStack::Log("Failed to create index buffer for segment.");
		return false;
	}
	this->indexBuffer = indexBuffer;
	return true;
}

// Free the index buffer resource.
void EntityModelSegment::FreeRendererResources(Renderer::Resources *resources)
{
	resources->DestroyIndexBuffer(indexBuffer);
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
	Renderer::Buffer *vertexBuffer = resources->CreateVertexBuffer(
		mesh.GetVertexBuffer(),
		mesh.GetVertexBufferSize(),
		vertexSchema);
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
	resources->DestroyVertexBuffer(vertexBuffer);
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
	// Allocate array of frames.
	EntityModelFrame *frames;
	if (!MemoryManager::AllocateArray(&frames, frameCount)) {
		return false;
	}
	EntityModelFrame *currentFrame = &frames[0];
	for (int i = 0; i < frameCount; ++i, ++currentFrame) {
		new (currentFrame) EntityModelFrame();
	}

	// Initialize the frames.
	currentFrame = &frames[0];
	for (int i = 0; i < frameCount; ++i, ++currentFrame) {
		if (!currentFrame->Initialize(vertexCount)) {
			return false;
		}
	}
	return true;
}
