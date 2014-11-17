#pragma once

#include "renderer/renderer_interface.h"
#include "common.h"

namespace OpenGL
{

	// OpenGL renderer implementation.
	class Implementation : public Renderer::Interface
	{

	public:

		// Initialize the renderer.
		virtual bool Initialize();

		// Shut down the renderer.
		virtual void Shutdown();

		// Clear scene for a new frame.
		virtual void ClearScene();

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