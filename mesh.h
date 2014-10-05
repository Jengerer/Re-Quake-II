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
	inline int GetVertexCount() const;
	inline VertexType *GetVertexBuffer();
	inline const VertexType *GetVertexBuffer() const;

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
	VertexType *vertices = reinterpret_cast<VertexType*>(MemoryManager::AllocateArray(sizeof(VertexType)));
	if (vertices == nullptr) {
		return false;
	}
	this->vertices = vertices;
	this->vertexCount = vertexCount;
}

// Allocate space for vertices.
template <typename VertexType>
int Mesh<VertexType>::GetVertexCount() const
{
	return vertexCount;
}

// Get mutable vertex reference.
template <typename VertexType>
VertexType *Mesh<VertexType>::GetVertex(int index)
{
	return vertices[index];
}

// Get immutable vertex reference.
template <typename VertexType>
const VertexType *Mesh<VertexType>::GetVertex(int index) const
{
	return vertices[index];
}

// Common vertex defines.
typedef Mesh<TexturedVertex> TexturedMesh;