#pragma once

#include "vector3.h"
#include "vector2.h"

// Class for representing a mesh of vertices.
template <typename VertexType>
class Mesh
{

public:

	Mesh();
	~Mesh();

	// Initialize for number of vertices.
	bool Initialize(int vertexCount);
	void Destroy();
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
	Destroy();
}

// Allocate space for vertices.
template <typename VertexType>
bool Mesh<VertexType>::Initialize(int vertexCount)
{
	unsigned int bufferSize = sizeof(VertexType) * vertexCount;
	VertexType *vertices = reinterpret_cast<VertexType*>(MemoryManager::Allocate(bufferSize));
	if (vertices == nullptr) {
		return false;
	}
	this->vertices = vertices;
	this->vertexCount = vertexCount;
	return true;
}

// Clear vertex array.
template <typename VertexType>
void Mesh<VertexType>::Destroy()
{
	if (vertices != nullptr) {
		MemoryManager::Free(vertices);
		vertices = nullptr;
	}
}