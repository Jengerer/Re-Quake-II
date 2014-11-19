#include "entity_model.h"
#include "error_stack.h"
#include "memory_manager.h"
#include <string.h>

// Quake II material layout.
const int StartFrameAttributeCount = 2;
const int EndFrameAttributeCount = StartFrameAttributeCount;
const int TextureAttributeCount = 1;
const Renderer::Attribute startFrameAttributes[StartFrameAttributeCount] =
{
	Renderer::Attribute("position0", Renderer::PositionType, Renderer::Vector3Type),
	Renderer::Attribute("normal0", Renderer::NormalType, Renderer::Vector3Type)
};
const Renderer::Attribute endFrameAttributes[EndFrameAttributeCount] = {
	Renderer::Attribute("position1", Renderer::PositionType, Renderer::Vector3Type),
	Renderer::Attribute("normal1", Renderer::NormalType, Renderer::Vector3Type)
};
const Renderer::Attribute textureAttributes[TextureAttributeCount] = {
	Renderer::Attribute("uv", Renderer::TextureCoordinateType, Renderer::Vector2Type)
};
const int StartFrameBufferIndex = 0;
const int EndFrameBufferIndex = 1;
const int TextureAttributeIndex = 2;
const int QuakeBufferCount = 1;
const Renderer::BufferLayout QuakeBufferLayouts[QuakeBufferCount] =
{
	Renderer::BufferLayout(startFrameAttributes, StartFrameAttributeCount),
	// Renderer::BufferLayout(endFrameAttributes, EndFrameAttributeCount),
	// Renderer::BufferLayout(textureAttributes, TextureAttributeCount)
};

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
}

// Initialize segment for index count.
void EntityModelSegment::SetParameters(const unsigned int *indices, int indexCount, Renderer::PrimitiveType type)
{
	this->indices = indices;
	this->indexCount = indexCount;
	this->type = type;
}

// Prepare the index buffer resource for this segment.
bool EntityModelSegment::LoadResources(Renderer::Resources *resources)
{
	// Create an index buffer with this data.
	int bufferSize = sizeof(int) * indexCount;
	Renderer::IndexBuffer *indexBuffer = resources->CreateIndexBuffer(indices, bufferSize, Renderer::UnsignedIntType);
	if (indexBuffer == nullptr) {
		ErrorStack::Log("Failed to create index buffer for segment.");
		return false;
	}
	this->indexBuffer = indexBuffer;
	return true;
}

// Make a draw call for this segment.
void EntityModelSegment::Draw(Renderer::Interface *renderer)
{
	renderer->SetIndexBuffer(indexBuffer);
	renderer->DrawIndexed(type, indexCount);
	renderer->UnsetIndexBuffer(indexBuffer);
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
void EntityModelFrame::SetVertices(const EntityModelVertex *vertices, int bufferSize)
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

// Singleton model material layout.
Renderer::MaterialLayout *EntityModel::layout = nullptr;

EntityModel::EntityModel()
	: frames(nullptr),
	frameCount(0),
	indices(nullptr),
	segments(nullptr),
	segmentCount(0)
{
}

EntityModel::~EntityModel()
{
	// Delete frame objects.
	if (frames != nullptr) {
		delete[] frames;
	}

	// Free index buffer.
	if (indices != nullptr) {
		MemoryManager::Free(indices);
	}

	// Delete segment objects.
	if (segments != nullptr) {
		delete[] segments;
	}
}

// Initialize entity model vertices and frames.
bool EntityModel::Initialize(int frameCount, int vertexCount)
{
	// Allocate frame objects.
	frames = new EntityModelFrame[frameCount];
	if (frames == nullptr) {
		ErrorStack::Log("Failed to allocate %d frame objects for model.", frameCount);
		return false;
	}
	this->frameCount = frameCount;

	// Allocate enough vertices for all frames.
	const int TotalVertices = frameCount * vertexCount;
	if (!mesh.Initialize(TotalVertices)) {
		ErrorStack::Log("Failed to allocate %d vertices for model.", TotalVertices);
		return false;
	}
	return true;
}

// Initialize the entity model indices.
bool EntityModel::InitializeSegments(int indexCount, int segmentCount)
{
	// Allocate index array.
	int bufferSize = indexCount * sizeof(int);
	unsigned int *indices = reinterpret_cast<unsigned int*>(MemoryManager::Allocate(bufferSize));
	if (indices == nullptr) {
		ErrorStack::Log("Failed to allocate %d indices for model index buffer.", indexCount);
		return false;
	}
	this->indices = indices;

	// Allocate segments array.
	segments = new EntityModelSegment[segmentCount];
	if (segments == nullptr) {
		ErrorStack::Log("Failed to allocate %d segment objects for model.", segmentCount);
		return false;
	}
	this->segmentCount = segmentCount;
	return true;
}

// Load the mesh in the renderer.
bool EntityModel::LoadResources(Renderer::Resources *resources)
{
	// Get index buffers for the segments.
	int count = segmentCount;
	EntityModelSegment *segment = segments;
	for (int i = 0; i < count; ++i, ++segment) {
		if (!segment->LoadResources(resources)) {
			return false;
		}
	}

	// Load the frames.
	count = frameCount;
	EntityModelFrame *frame = frames;
	for (int i = 0; i < count; ++i, ++frame) {
		if (!frame->LoadResources(resources)) {
			return false;
		}
	}
	return true;
}

// Draw the mesh.
void EntityModel::Draw(Renderer::Interface *renderer)
{
	// Bind the frame and layout.
	Renderer::Buffer *buffer = frames[0].GetVertexBuffer();
	layout->BindBuffer(StartFrameBufferIndex, buffer);
	renderer->SetMaterialLayout(layout);

	// Draw all segments.
	int count = segmentCount;
	EntityModelSegment *segment = segments;
	for (int i = 0; i < count; ++i, ++segment) {
		segment->Draw(renderer);
	}

	// Clear layout.
	renderer->UnsetMaterialLayout(layout);
}

// Prepare the entity for rendering.
bool EntityModel::LoadStaticResources(Renderer::Resources *resources, Renderer::Material *modelMaterial)
{
	// Get the material layout.
	Renderer::MaterialLayout *layout = modelMaterial->GetLayout(QuakeBufferLayouts, QuakeBufferCount);
	if (layout == nullptr) {
		return false;
	}
	EntityModel::layout = layout;
	return true;
}

// Free static resources.
void EntityModel::FreeStaticResources()
{
	// Free layout if we have one.
	if (layout != nullptr) {
		layout->Destroy();
	}
}
