#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "renderer.h"
#include "window.h"

// Struct that refers to generic game/engine configuration state.
typedef struct engine_configuration
{
	const char* title;
	int width;
	int height;
} engine_configuration_t;

// Engine state struct.
typedef struct engine
{
	window_t window;
	renderer_t renderer;
	engine_configuration_t config;
} engine_t;

// Engine initialization/destruction.
void null_engine(engine_t *engine);
int initialize_engine(engine_t *engine);
void destroy_engine(engine_t *engine);

// Engine run functions.
int run_engine(engine_t *engine);

#endif // _ENGINE_H_