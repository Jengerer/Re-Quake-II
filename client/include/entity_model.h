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
typedef Mesh<Vector2> EntityModelTextureCoordinates;

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
	bool Initialize(int frameCount, int triangleCount);

	// Shut down entity model.
	void Destroy();

	// Load the renderer resources.
	bool LoadResources(Renderer::Resources *resources);

	// Pass the model for rendering.
	void Draw(Renderer::Interface *renderer);

	// Model buffer functions.
	inline EntityModelFrame *GetFrames() { return frames; }
	inline int GetFrameCount() const { return frameCount; }
	inline EntityModelMesh *GetMesh() { return &mesh; }
	inline EntityModelTextureCoordinates *GetTextureCoordinates() { return &textureCoordinates; }

public:

	// Get material layout for models.
	static bool LoadStaticResources(Renderer::Resources *resources, Renderer::Material *modelMaterial);
	static void FreeStaticResources();

private:

	// Frames of the model's animations (and their vertex buffers).
	EntityModelMesh mesh;
	EntityModelFrame *frames;
	int frameCount;

	// Array of texture coordinates.
	EntityModelTextureCoordinates textureCoordinates;
	Renderer::Buffer *textureCoordinateBuffer;

private:

	// Helper constant for loading data.
	static const int VerticesPerTriangle = 3;

	// Model-generic material layout.
	static Renderer::MaterialLayout *layout;

};