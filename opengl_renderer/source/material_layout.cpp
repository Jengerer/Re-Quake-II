#include "material_layout.h"
#include <error_stack.h>
#include <memory_manager.h>

namespace OpenGL
{

	MaterialLayout::MaterialLayout() : bufferLayouts(0), bufferCount(0)
	{
	}

	MaterialLayout::~MaterialLayout()
	{
        delete[] bufferLayouts;
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

}
