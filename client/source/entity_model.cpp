#include "entity_model.h"
#include <error_stack.h>
#include <memory_manager.h>
#include <string.h>

// Quake II model material layout.
const int StartFrameAttributeCount = 2;
const int EndFrameAttributeCount = StartFrameAttributeCount;
const int TextureAttributeCount = 1;
const Renderer::Attribute StartFrameAttributes[StartFrameAttributeCount] =
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
const int TextureCoordinateBufferIndex = 2;
const int QuakeBufferCount = 3;
const Renderer::BufferLayout QuakeBufferLayouts[QuakeBufferCount] =
{
	Renderer::BufferLayout(StartFrameAttributes, StartFrameAttributeCount),
	Renderer::BufferLayout(endFrameAttributes, EndFrameAttributeCount),
	Renderer::BufferLayout(textureAttributes, TextureAttributeCount)
};

EntityModelFrame::EntityModelFrame() : vertices(nullptr), bufferSize(0), vertexBuffer(nullptr)
{
}

EntityModelFrame::~EntityModelFrame()
{
    delete vertexBuffer;
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
	frameCount(0)
{
}

EntityModel::~EntityModel()
{
	Destroy();
}

// Initialize entity model vertices and frames.
bool EntityModel::Initialize(int frameCount, int triangleCount)
{
	// Allocate frame objects.
	frames = new EntityModelFrame[frameCount];
	if (frames == nullptr) {
		ErrorStack::Log("Failed to allocate %d frame objects for model.", frameCount);
		return false;
	}
	this->frameCount = frameCount;

	// Allocate enough vertices for all frames.
	const int VerticesPerFrame = triangleCount * VerticesPerTriangle;
	const int TotalVertices = frameCount * VerticesPerFrame;
	if (!mesh.Initialize(TotalVertices)) {
		ErrorStack::Log("Failed to allocate %d vertices for model.", TotalVertices);
		return false;
	}

	// Allocate texture coordinates for triangles.
	if (!textureCoordinates.Initialize(VerticesPerFrame)) {
		ErrorStack::Log("Failed to allocate %d texture coordinates for model.", TotalVertices);
		return false;
	}
	return true;
}

// Free all model resources.
void EntityModel::Destroy()
{
	delete[] frames;
	frames = nullptr;
	mesh.Destroy();
	textureCoordinates.Destroy();
}

// Load the mesh in the renderer.
bool EntityModel::LoadResources(Renderer::Resources *resources)
{
	int count = frameCount;
	EntityModelFrame *frame = frames;
	for (int i = 0; i < count; ++i, ++frame) {
		if (!frame->LoadResources(resources)) {
			return false;
		}
	}

	// Load texture coordinates into a buffer.
	textureCoordinateBuffer = resources->CreateBuffer(
		textureCoordinates.GetVertexBuffer(),
		textureCoordinates.GetVertexBufferSize());
	if (textureCoordinateBuffer == nullptr) {
		ErrorStack::Log("Failed to create texture coordinate buffer for model.");
		return false;
	}
	return true;
}

// Draw the mesh.
void EntityModel::Draw(Renderer::Interface *renderer)
{
	layout->BindBuffer(StartFrameBufferIndex, frames[0].GetVertexBuffer());
	layout->BindBuffer(EndFrameBufferIndex, frames[1].GetVertexBuffer());
	layout->BindBuffer(TextureCoordinateBufferIndex, textureCoordinateBuffer);
	renderer->SetMaterialLayout(layout);

	// Number of vertices per frame matches texture coordinate count.
	renderer->Draw(Renderer::Triangles, textureCoordinates.GetVertexCount());
	renderer->UnsetMaterialLayout(layout);
}

// Prepare the entity for rendering.
bool EntityModel::LoadStaticResources(Renderer::Resources *resources, Renderer::Material *modelMaterial)
{
	layout = modelMaterial->GetLayout(QuakeBufferLayouts, QuakeBufferCount);
	if (layout == nullptr) {
		return false;
	}
	return true;
}

// Free model-generic renderer resources.
void EntityModel::FreeStaticResources()
{
    delete layout;
    layout = nullptr;
}
