#pragma once

#include "allocatable.h"
#include "buffer.h"
#include "buffer_layout.h"
#include "renderer/material_layout_interface.h"

namespace OpenGL
{

	// Object for representing multiple buffer layouts in a material.
	class MaterialLayout : public Renderer::MaterialLayout, public Allocatable
	{

	public:

		MaterialLayout();

		// Set the buffer layouts in this material layout.
		void SetBufferLayouts(BufferLayout *bufferLayouts, int bufferCount);

		// Initialize the material count for the number of buffers.
		bool Initialize();

		// Destroy this layout.
		virtual void Destroy();

		// Bind a buffer to a specific layout.
		virtual void BindBuffer(int bufferIndex, const Renderer::Buffer *buffer);

		// Activate this layout for rendering.
		void Activate();

		// Deactivate this layout from rendering.
		void Deactivate();

	private:

		// Private destructor; must be killed through Destroy();
		~MaterialLayout();

	private:

		// Array of OpenGL buffers for each layout.
		BufferLayout *bufferLayouts;
		int bufferCount;
		
	};

}