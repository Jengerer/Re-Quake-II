#pragma once

#include "window.h"
#include <renderer/renderer_interface.h>
#include <renderer/resources_interface.h>

namespace Engine
{

	// Interface by which an engine module can request resources from the engine.
	class Utilities
	{

	public:

		// Create a window with a given set of parameters.
		virtual Window *MakeWindow(const char *title, int width, int height, WindowFlags flags) = 0;

		// Get the renderer interface.
		virtual Renderer::Interface *GetRenderer() = 0;

		// Get the renderer resource interface.
		virtual Renderer::Resources *GetRendererResources() = 0;

	};

}