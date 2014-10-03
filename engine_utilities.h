#pragma once

#include "renderer.h"
#include "renderer_resources.h"

// Interface by which an engine module can request resources from the engine.
class EngineUtilities
{

public:

	// Create a window with a given set of parameters.
	virtual bool CreateWindow(const char *title, int width, int height, int flags) = 0;

	// Update the window's parameters.
	virtual bool UpdateWindow(int width, int height, int flags) = 0;

	// Swap the window frame buffer.
	virtual void SwapBuffers() = 0;

	// Get the renderer interface.
	virtual Renderer::Interface *GetRenderer();

};