#ifndef _ENGINE_UTILITIES_H_
#define _ENGINE_UTILITIES_H_

#include "renderer.h"
#include "renderer_shading_resources.h"

// Interface by which an engine module can request resources from the engine.
class EngineUtilities
{

public:

// Window and display functions.

	// Create a window with a given set of parameters.
	virtual bool CreateWindow(const char *title, int width, int height, int flags) = 0;
	// Update the window's parameters.
	virtual bool UpdateWindow(int width, int height, int flags) = 0;
	// Swap the window frame buffer.
	virtual void SwapBuffers() = 0;

// Rendering utility functions.

	// Get the renderer interface.
	virtual Renderer 


};
// Create the window.
typedef int (*engine_create_window_t)(
	const char *title,
	int width,
	int height,
	int flags);

// Create/update window size and appearance.
typedef int (*engine_update_window_t)(
	int width,
	int height,
	int flags);

// Window buffer swap.
typedef void (*engine_swap_buffers_t)(void);

// Get the engine renderer interface.
typedef const renderer_t *(*engine_get_renderer_t)(void);

// Get the engine renderer shader utilities.
typedef const renderer_shader_utilities_t *(*engine_get_shader_utilities_t)(void);

// Engine functions and utilities to be called by game/renderer.
typedef struct engine_utilities
{
	// Engine display functions.
	engine_create_window_t create_window;
	engine_update_window_t update_window;
	engine_swap_buffers_t swap_buffers;

	// Rendering related functions.
	engine_get_renderer_t get_renderer;
	engine_get_shader_utilities_t get_shader_utilities;
} engine_utilities_t;

#endif // _ENGINE_UTILITIES_H_