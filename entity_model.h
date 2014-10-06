#pragma once

#include "mesh.h"
#include "renderer_shared.h"
#include "renderer_resources.h"

// Entity model segment that's rendered 
class EntityModelSegment
{

public:

	EntityModelSegment();

	// Initialize segment for vertices.
	bool Initialize(int vertexCount);
	inline TexturedMesh *GetMesh();

	// Set model type.
	void SetModelType(Renderer::ModelType modelType);

	// Set renderer model.
	void SetModel(Renderer::Model *model);
	void DestroyModel(Renderer::Resources *resources);
	inline const Renderer::Model *GetModel() const;

private:

	// Mesh for this segment.
	TexturedMesh mesh;

	// Renderable element of this segment.
	Renderer::Model *model;
	Renderer::ModelType modelType;

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
	bool Initialize(int segmentCount,
		int vertexCount);

	// Set frame name.
	void SetFrameName(const char frameName[FrameNameLength]);

	// Get the segment buffer for this frame.
	inline EntityModelSegment *GetSegments();

private:

	char frameName[FrameNameLength];
	EntityModelSegment *segments;
	int segmentCount;

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

private:

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