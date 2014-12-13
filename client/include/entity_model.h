#pragma once

#include <allocatable.h>
#include <mesh.h>
#include <renderer/renderer_interface.h>
#include <renderer/shared.h>
#include <renderer/resources_interface.h>

// Entity model vertex type.
struct EntityModelVertex
{
	Vector3 position;
	Vector3 normal;
};
typedef Mesh<EntityModelVertex> EntityModelMesh;

// Entity model segment that's rendered as triangle strip or fan.
class EntityModelSegment : public Allocatable
{

public:

	EntityModelSegment();
	~EntityModelSegment();

	// Set up entity model segment.
	void SetParameters(const unsigned int *indices, int indexCount, Renderer::PrimitiveType type);

	// Loading and cleaning renderer resources.
	bool LoadResources(Renderer::Resources *resources);

	// Make a draw call for this segment.
	void Draw(Renderer::Interface *renderer);

private:

	// Index data buffer.
	const unsigned int *indices;
	int indexCount;

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
	void SetVertices(const EntityModelVertex *vertices, int vertexCount);

	// Handle renderer resources.
	bool LoadResources(Renderer::Resources *resources);

	// Get the vertex buffer for this frame.
	inline Renderer::Buffer *GetVertexBuffer() { return vertexBuffer; }

	// Set frame name.
	void SetFrameName(const char frameName[FrameNameLength]);

private:

	char frameName[FrameNameLength];

	// Vertex data for this frame.
	const EntityModelVertex *vertices;
	int bufferSize;

	// Renderer resource for this frame.
	Renderer::Buffer *vertexBuffer;

};

// Animated model for an entity.
class EntityModel
{

public:

	EntityModel();
	~EntityModel();

	// Allocate vertices and the frames.
	bool Initialize(int frameCount, int vertexCount);

	// Allocate objects for the segments.
	bool InitializeSegments(int indexCount, int segmentCount);

	// Shut down entity model.
	void Destroy();

	// Load the renderer resources.
	bool LoadResources(Renderer::Resources *resources);

	// Pass the model for rendering.
	void Draw(Renderer::Interface *renderer);

	// Get buffer of frames.
	inline EntityModelFrame *GetFrames() { return frames; }
	inline int GetFrameCount() const { return frameCount; }

	// Get buffer of segments.
	inline unsigned int *GetIndexData() const { return indices; }
	inline EntityModelSegment *GetSegments() { return segments; }
	inline int GetSegmentCount() const { return segmentCount; }

	// Get mesh of vertices.
	inline EntityModelMesh *GetMesh() { return &mesh; }

public:

	// Get material layout for models.
	static bool LoadStaticResources(Renderer::Resources *resources, Renderer::Material *modelMaterial);
	static void FreeStaticResources();

private:

	// Frames of the model's animations (and their vertex buffers).
	EntityModelMesh mesh;
	EntityModelFrame *frames;
	int frameCount;

	// Array of model segments (and their index buffers).
	unsigned int *indices;
	EntityModelSegment *segments;
	int segmentCount;

private:

	// Model-generic material layout.
	static Renderer::MaterialLayout *layout;

};