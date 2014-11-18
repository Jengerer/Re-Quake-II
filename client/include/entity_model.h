#pragma once

#include "allocatable.h"
#include "mesh.h"
#include "renderer/renderer_interface.h"
#include "renderer/shared.h"
#include "renderer/resources_interface.h"

// Entity model segment that's rendered as triangle strip or fan.
class EntityModelSegment : public Allocatable
{

public:

	EntityModelSegment();
	~EntityModelSegment();

	// Initialize segment for vertices.
	bool Initialize(int indexCount, Renderer::PrimitiveType type);
	inline int *GetIndexData() { return indices; }

	// Loading and cleaning renderer resources.
	bool LoadResources(Renderer::Resources *resources);

	// Make a draw call for this segment.
	void Draw(Renderer::Interface *renderer);

private:

	// Index data buffer.
	int *indices;
	int indexCount;
	unsigned int arraySize;

	// Renderer parameters for this segment.
	Renderer::IndexBuffer *indexBuffer;
	Renderer::PrimitiveType type;

};

// Entity model frame.
class EntityModelFrame : public Allocatable
{

private:

	// Maximum frame name length.
	static const int FrameNameLength = 16;

public:

	EntityModelFrame();
	~EntityModelFrame();

	// Set the reference for the vertex data.
	void SetVertices(const TexturedVertex *vertices, int vertexCount);

	// Handle renderer resources.
	bool LoadVertices(const TexturedVertex *vertices,
		int vertexCount,
		Renderer::Resources *resources);
	void Precache(Renderer::Resources *resources);
	inline Renderer::Buffer *GetVertexBuffer() { return vertexBuffer; }

	// Set frame name.
	void SetFrameName(const char frameName[FrameNameLength]);

private:

	char frameName[FrameNameLength];

	// Vertex data for this frame.
	const TexturedVertex *vertices;
	int vertexCount;

	// Renderer resource for this frame.
	Renderer::Buffer *vertexBuffer;

};

// Animated model for an entity.
class EntityModel
{

public:

	EntityModel();
	~EntityModel();

	// Allocate objects for the frames.
	bool Initialize(
		int frameCount,
		int segmentCount,
		int vertexCount);

	// Get buffer of frames.
	inline EntityModelFrame *GetFrames() { return frames; }
	inline int GetFrameCount() const { return frameCount; }

	// Get buffer of segments.
	inline EntityModelSegment *GetSegments() { return segments; }
	inline int GetSegmentCount() const { return segmentCount; }

public:

	// Get material layout for models.
	static void SetMaterialLayout(Renderer::MaterialLayout *layout);
	static void FreeMaterialLayout();

private:

	// Array of model segments (and their index buffers).
	EntityModelSegment *segments;
	int segmentCount;

	// Frames of the model's animations (and their vertex buffers).
	TexturedMesh vertices;
	EntityModelFrame *frames;
	int frameCount;

private:

	// Model-generic material layout.
	static Renderer::MaterialLayout *layout;

};

// Get vertex buffer for segment.
TexturedMesh *EntityModelSegment::GetMesh()
{
	return &mesh;
}

// Get segment model.
const Renderer::Model *EntityModelSegment::GetModel() const
{
	return model;
}

// Get segment buffer for frame.
EntityModelSegment *EntityModelFrame::GetSegments()
{
	return segments;
}

// Get frame buffer for model.
EntityModelFrame *EntityModel::GetFrames()
{
	return frames;
}
