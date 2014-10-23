#pragma once

#include "vector3.h"
#include "vector2.h"

// Mesh vertex structure.
struct TexturedVertex
{
	Vector3 position;
	Vector2 texture;
};

// Class for representing a mesh of vertices.
template <typename VertexType>
class Mesh
{

public:

	Mesh();
	~Mesh();

	// Initialize for number of vertices.
	bool Initialize(int vertexCount);
	inline int GetVertexCount() const { return vertexCount; }
	inline int GetVertexBufferSize() const { return GetVertexCount * sizeof(VertexType); }
	inline VertexType *GetVertexBuffer() { return vertices; }
	inline const VertexType *GetVertexBuffer() const { return vertices; }

private:

	VertexType *vertices;
	int vertexCount;

};

template <typename VertexType>
Mesh<VertexType>::Mesh()
	: vertices(nullptr), vertexCount(0)
{
}

// Destroy vertex buffer.
template <typename VertexType>
Mesh<VertexType>::~Mesh()
{
	if (vertices != nullptr) {
		MemoryManager::Free(vertices);
	}
}

// Allocate space for vertices.
template <typename VertexType>
bool Mesh<VertexType>::Initialize(int vertexCount)
{
	VertexType *vertices;
	if (!MemoryManager::AllocateArray(&vertices, vertexCount)) {
		return false;
	}
	this->vertices = vertices;
	this->vertexCount = vertexCount;
	return true;
}

// Allocate space for vertices.
template <typename VertexType>
int Mesh<VertexType>::GetVertexCount() const
{
	return vertexCount;
}

// Get mutable vertex references.
template <typename VertexType>
VertexType *Mesh<VertexType>::GetVertexBuffer()
{
	return vertices;
}

// Get immutable vertex references.
template <typename VertexType>
const VertexType *Mesh<VertexType>::GetVertexBuffer() const
{
	return vertices;
}

// Common vertex defines.
typedef Mesh<TexturedVertex> TexturedMesh;
