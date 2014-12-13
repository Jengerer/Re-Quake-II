#pragma once

#include "common.h"
#include "index_buffer.h"
#include <renderer/renderer_interface.h>

namespace OpenGL
{

	// OpenGL renderer implementation.
	class Implementation : public Renderer::Interface
	{

	public:

		// Initialize the renderer.
		virtual bool Initialize();

		// Shut down the renderer.
		virtual void Destroy();

		// Clear scene for a new frame.
		virtual void ClearScene();

		// Set the material to render with.
		virtual void SetMaterial(Renderer::Material *material);

		// Unset material from rendering.
		virtual void UnsetMaterial(Renderer::Material *material);

		// Activate a material layout.
		virtual void SetMaterialLayout(Renderer::MaterialLayout *layout);

		// Deactivate a material layout.
		virtual void UnsetMaterialLayout(Renderer::MaterialLayout *layout);

		// Set index buffer for rendering.
		virtual void SetIndexBuffer(Renderer::IndexBuffer *indexBuffer);

		// Unset index buffer from rendering.
		virtual void UnsetIndexBuffer(Renderer::IndexBuffer *indexBuffer);

		// Draw non-indexed primitive.
		virtual void Draw(Renderer::PrimitiveType type, unsigned int count);

		// Draw indexed primitive.
		virtual void DrawIndexed(Renderer::PrimitiveType type, unsigned int indexCount);

	public:

		// Singleton retriever.
		static Implementation *GetInstance();

	private:

		// Private constructor/destructor for singleton.
		Implementation();
		~Implementation();

	private:

		// Index type for bound index buffer.
		GLenum indexType;

	private:

		// Singleton instance.
		static Implementation instance;

	};

}