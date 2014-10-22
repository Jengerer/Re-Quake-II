#include "entity_model.h"
#include "error_stack.h"
#include "memory_manager.h"
#include <string.h>

EntityModelSegment::EntityModelSegment(Renderer::GeometryType geometryType)
	: indices(nullptr),
	vertices(nullptr),
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

// Set the reference to the data holding this segment's vertex data.
void EntityModelSegment::SetModel(Renderer::Model *model)
{
	this->model = model;
}

// Destroy the model.
void EntityModelSegment::DestroyModel(Renderer::Resources *resources)
{
	resources->DestroyModel(model);
}

EntityModelFrame::EntityModelFrame() : segments(nullptr)
{
}

EntityModelFrame::~EntityModelFrame()
{
	if (segments != nullptr) {
		MemoryManager::DestroyArray(segments);
	}
}

// Initialize model frame.
bool EntityModelFrame::Initialize(
	int segmentCount,
	int vertexCount)
{
	// Allocate segments.
	EntityModelSegment *segments;
	if (!MemoryManager::AllocateArray(&segments, segmentCount)) {
		return false;
	}
	this->segments = segments;
	this->segmentCount = segmentCount;

	// Initialize each segment.
	EntityModelSegment *currentSegment = &segments[0];
	for (int i = 0; i < segmentCount; ++i, ++currentSegment) {
		if (!currentSegment->Initialize(vertexCount)) {
			return false;
		}
	}
	return true;
}

// Copy frame name.
void EntityModelFrame::SetFrameName(const char frameName[FrameNameLength])
{
	// Copy name.
	strncpy(this->frameName, frameName, FrameNameLength);
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

	// Initialize the frames.
	EntityModelFrame *currentFrame = &frames[0];
	for (int i = 0; i < frameCount; ++i, ++currentFrame) {
		if (!currentFrame->Initialize(segmentCount, vertexCount)) {
			return false;
		}
	}
	return true;
}
