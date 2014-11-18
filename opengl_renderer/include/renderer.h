#pragma once

#include "common.h"
#include "renderer/renderer_interface.h"

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

	public:

		// Singleton retriever.
		static Implementation *GetInstance();

	private:

		// Private constructor/destructor for singleton.
		Implementation();
		~Implementation();

	private:

		// Singleton instance.
		static Implementation instance;

	};

}