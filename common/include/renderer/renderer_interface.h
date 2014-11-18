#pragma once

#include "shared.h"
#include "renderer/index_buffer_interface.h"
#include "renderer/material_interface.h"
#include "renderer/material_layout_interface.h"

namespace Renderer
{

	// Renderer drawing interface.
	class Interface
	{

	public:

		// Initialize renderer.
		virtual bool Initialize() = 0;

		// Shut down renderer.
		virtual void Destroy() = 0;

		// Clear the scene for a new frame.
		virtual void ClearScene() = 0;

		// Set the material to render with.
		virtual void SetMaterial(Renderer::Material *material) = 0;

		// Unset a material from rendering.
		virtual void UnsetMaterial(Renderer::Material *material) = 0;

		// Activate a material layout.
		virtual void SetMaterialLayout(MaterialLayout *layout) = 0;

		// Deactivate a material layout.
		virtual void UnsetMaterialLayout(MaterialLayout *layout) = 0;

		// Set the index buffer to render with.
		virtual void SetIndexBuffer(IndexBuffer *indexBuffer) = 0;

		// Unset the index buffer from rendering.
		virtual void UnsetIndexBuffer(IndexBuffer *indexBuffer) = 0;

		// Draw primitive without indices.
		virtual void Draw(PrimitiveType type, unsigned int count) = 0;

		// Draw primitive with indices.
		virtual void DrawIndexed(PrimitiveType type, unsigned int indexCount) = 0;

	};

}