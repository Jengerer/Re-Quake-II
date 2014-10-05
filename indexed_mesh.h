#pragma once

#include "mesh.h"

// Mesh class that has vertex indices for draw order.
template <typename VertexType>
class IndexedMesh : public Mesh<VertexType>
{

public:

	IndexedMesh();
	~IndexedMesh();

	// Initialize for vertex and index count.
	bool Initialize(int vertexCount, int indexCount);

	// Set a given vertex index.
	inline int GetIndexCount() const;
	inline unsigned int *GetIndexBuffer();
	inline const unsigned int *GetIndexBuffer() const;

private:

	unsigned int *indices;
	int indexCount;

};

template <typename VertexType>
IndexedMesh<VertexType>::IndexedMesh()
	: indices(nullptr), indexCount(0)
{
}

template <typename VertexType>
IndexedMesh<VertexType>::~IndexedMesh()
{
	if (indices != nullptr) {
		MemoryManager::Free(indices);
	}
}

// Set up index and vertex buffer.
template <typename VertexType>
bool IndexedMesh<VertexType>::Initialize(int vertexCount, int indexCount)
{
	// Initialize vertices.
	if (!Mesh<VertexType>::Initialize(vertexCount)) {
		return false;
	}

	// Allocate space for indices.
	unsigned int *indices = reinterpret_cast<unsigned int*>(MemoryManager::AllocateArray(sizeof(unsigned int), indexCount));
	if (indices == nullptr) {
		return false;
	}
	this->indices = indices;
	this->indexCount = indexCount;
	return true;
}

// Get number of indices.
template <typename VertexType>
int IndexedMesh<VertexType>::GetIndexCount() const
{
	return indexCount;
}

// Get mutable index buffer.
template <typename VertexType>
unsigned int *IndexedMesh<VertexType>::GetIndexBuffer()
{
	return indices;
}

// Get immutable index buffer.
template <typename VertexType>
const unsigned int *IndexedMesh<VertexType>::GetIndexBuffer() const
{
	return indices;
}