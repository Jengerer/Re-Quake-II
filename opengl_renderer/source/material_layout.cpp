#pragma once

#include "error_stack.h"
#include "material_layout.h"
#include "memory_manager.h"

namespace OpenGL
{

	MaterialLayout::MaterialLayout() : bufferLayouts(0), bufferCount(0)
	{
	}

	// Set the buffer layouts array.
	void MaterialLayout::SetBufferLayouts(BufferLayout *bufferLayouts, int bufferCount)
	{
		this->bufferLayouts = bufferLayouts;
		this->bufferCount = bufferCount;
	}

	// Initialize the layout.
	bool MaterialLayout::Initialize()
	{
		return true;
	}

	// Destroy this layout.
	void MaterialLayout::Destroy()
	{
	}

	// Bind a buffer to a layout slot.
	void MaterialLayout::BindBuffer(int bufferIndex, const Renderer::Buffer *buffer)
	{
		// Bind the buffer and activate the matching layout.
		const Buffer *glBuffer = static_cast<const Buffer*>(buffer);
		glBuffer->Bind();
		bufferLayouts[bufferIndex].Activate();
		glBuffer->Unbind();
	}

	// Activate this material layout.
	void MaterialLayout::Activate()
	{
		// No need to do anything, all buffers are bound.
	}

	// Deactivate this material layout.
	void MaterialLayout::Deactivate()
	{
		// Deactivate all layouts.
		BufferLayout *bufferLayouts = this->bufferLayouts;
		int bufferCount = this->bufferCount;
		for (int i = 0; i < bufferCount; ++i, ++bufferLayouts) {
			bufferLayouts->Deactivate();
		}
	}

	// Private destructor; must be killed through Destroy().
	MaterialLayout::~MaterialLayout()
	{
		// Delete the buffer layouts.
		if (bufferLayouts != nullptr) {
			delete[] bufferLayouts;
		}
	}

}
