#pragma once

#include "error_stack.h"
#include "material_layout.h"
#include "memory_manager.h"

namespace OpenGL
{

	MaterialLayout::MaterialLayout() : buffers(0), bufferLayouts(0), bufferCount(0)
	{
	}

	// Set the buffer layouts array.
	void MaterialLayout::SetBufferLayouts(BufferLayout *bufferLayouts, int bufferCount)
	{
		this->bufferLayouts = bufferLayouts;
		this->bufferCount = bufferCount;
	}

	// Set the buffer layouts for this material layout.
	bool MaterialLayout::Initialize()
	{
		// Allocate an array for the GL buffer pointers.
		size_t arraySize = bufferCount * sizeof(const Buffer*);
		const Buffer **buffers = reinterpret_cast<const Buffer**>(MemoryManager::Allocate(arraySize));
		if (buffers == nullptr) {
			ErrorStack::Log("Failed to allocate material buffers array.");
			return false;
		}
		this->buffers = buffers;
		return true;
	}

	// Destroy this layout.
	void MaterialLayout::Destroy()
	{
		MemoryManager::Free(buffers);
	}

	// Bind a buffer to a layout slot.
	void MaterialLayout::BindBuffer(int bufferIndex, const Renderer::Buffer *buffer)
	{
		buffers[bufferIndex] = static_cast<const Buffer*>(buffer);
	}

	// Activate this material layout.
	void MaterialLayout::Activate()
	{
		// Activate each buffer and the matching layout.
		const Buffer **buffers = this->buffers;
		BufferLayout *bufferLayouts = this->bufferLayouts;
		int bufferCount = this->bufferCount;
		for (int i = 0; i < bufferCount; ++i, ++buffers, ++bufferLayouts) {
			const Buffer *currentBuffer = *buffers;
			currentBuffer->Bind();
			bufferLayouts->Activate();
		}
	}

	// Deactivate this material layout.
	void MaterialLayout::Deactivate()
	{
		// Deactivate each buffer and the matching layout.
		const Buffer **buffers = this->buffers;
		BufferLayout *bufferLayouts = this->bufferLayouts;
		int bufferCount = this->bufferCount;
		for (int i = 0; i < bufferCount; ++i, ++buffers, ++bufferLayouts) {
			const Buffer *currentBuffer = *buffers;
			bufferLayouts->Deactivate();
			currentBuffer->Unbind();
		}
	}

	// Private destructor; must be killed through Destroy().
	MaterialLayout::~MaterialLayout()
	{
		// Delete the arrays.
		if (buffers != nullptr) {
			MemoryManager::Free(buffers);
		}
		if (bufferLayouts != nullptr) {
			delete[] bufferLayouts;
		}
	}

}