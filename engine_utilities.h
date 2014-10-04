#pragma once

#include "renderer.h"
#include "renderer_resources.h"
#include "window_shared.h"

namespace Engine
{

	// Interface by which an engine module can request resources from the engine.
	class Utilities
	{

	public:

		// Create a window with a given set of parameters.
		virtual bool CreateWindow(const char *title, int width, int height, WindowFlags flags) = 0;

		// Update the window's size.
		virtual bool ResizeWindow(int width, int height) = 0;

		// Update window flags.
		virtual bool UpdateWindowFlags(WindowFlags flags) = 0;

		// Swap the window frame buffer.
		virtual void SwapBuffers() = 0;

		// Get the renderer interface.
		virtual Renderer::Interface *GetRenderer();

		// Get the renderer resource interface.
		virtual Renderer::Resources *GetRendererResources();

	};

}