#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "sdl_window.h"
#include "renderer.h"
#include "engine_listener.h"

// Engine structure.
typedef struct engine
{
	window_t window;
	renderer_t renderer;
	engine_listener_t listener;
} engine_t;

// Singleton engine reference.
extern engine_t engine;

// Engine interface functions.
int engine_initialize(void);
void engine_shutdown(void);
int engine_run(void);

// Engine utility functions.
void engine_update_window(int width, int height, int flags);

#endif // _ENGINE_H_