#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "sdl_window.h"
#include "renderer.h"
#include "renderer_resources.h"
#include "engine_listener.h"

// Engine structure.
typedef struct engine
{
	sdl_window_t window;
	renderer_t renderer;
	renderer_shader_utilities_t shaders;
	engine_listener_t listener;
} engine_t;

// Singleton engine reference.
extern engine_t engine;

// Engine interface functions.
int engine_initialize(void);
void engine_shutdown(void);
int engine_run(void);

// Engine utility functions.
int engine_create_window(
	const char *title,
	int width,
	int height,
	int flags);
int engine_update_window(int width, int height, int flags);
void engine_swap_buffers(void);
const renderer_t *engine_get_renderer(void);
const renderer_shader_utilities_t *engine_get_shader_utilities(void);

#endif // _ENGINE_H_