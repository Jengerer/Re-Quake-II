#pragma once

#include "mesh.h"
#include "renderer_shared.h"
#include "renderer_resources.h"

// Entity model segment that's rendered as triangle strip or fan.
class EntityModelSegment
{

public:

	EntityModelSegment(Renderer::GeometryType geometryType);

	// Initialize segment for vertices.
	bool Initialize(int indexCount);
	inline int *GetIndexData() { return indices; }

	// Loading and cleaning renderer resources.
	bool LoadRendererResources(Renderer::Resources *resources);
	void FreeRendererResources(Renderer::Resources *resources);
	inline const Renderer::IndexBuffer *GetIndexBuffer() const { return vertices; }

private:

	// Index data buffer.
	int *indices;
	int indexCount;

	// Renderer parameters for this segment.
	Renderer::IndexBuffer *indexBuffer;
	Renderer::GeometryType type;

};

// Entity model frame.
class EntityModelFrame
{

private:

	// Maximum frame name length.
	static const int FrameNameLength = 16;

public:

	EntityModelFrame();
	~EntityModelFrame();

	// Initialize model frame.
	bool Initialize(int vertexCount);

	// Get vertex data.
	inline TexturedMesh *GetMesh() { return &mesh; }

	// Handle renderer resources.
	bool LoadRendererResources(Renderer::Resources *resources);
	void FreeRendererResources(Renderer::Resources *resources);
	inline Renderer::Buffer *GetVertexBuffer() { return vertexBuffer; }

	// Set frame name.
	void SetFrameName(const char frameName[FrameNameLength]);

public:

	// Prepare model-generic renderer resources.
	static void SetBufferSchema(Renderer::BufferSchema *vertexSchema);
	static void FreeBufferSchema(Renderer::Resources *resources);

private:

	// Buffer schema for the model.
	static Renderer::BufferSchema *vertexSchema;

private:

	char frameName[FrameNameLength];

	// Vertex data for this frame.
	TexturedMesh mesh;

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
	inline EntityModelFrame *GetFrames();
	inline int GetFrameCount() const { return frameCount; }

	// Get buffer of segments.
	inline EntityModelSegment *GetSegments();
	inline int GetSegmentCount() const { return segmentCount; }

private:

	// Array of model segments (and their index buffers).
	EntityModelSegment *segments;
	int segmentCount;

	// Frames of the model's animations (and their vertex buffers).
	EntityModelFrame *frames;
	int frameCount;

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
