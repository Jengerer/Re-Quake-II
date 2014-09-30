#include "game_manager.h"

// Singleton instance.
game_manager_t game_manager;

// TODO: Erase these once we get settings.
#define GAME_TITLE "Game"
#define GAME_WIDTH 1024
#define GAME_HEIGHT 768
#define GAME_FLAGS 0

// Null the game manager and its modules.
void game_manager_null(void)
{
	// Tell modules to null.
	game_manager.client.on_null();
}

// Initialize game manager.
int game_manager_initialize(void)
{
	const renderer_t *renderer;

	// Tell engine to create window and renderer.
	if (!game_manager.engine.create_window(GAME_TITLE, GAME_WIDTH, GAME_HEIGHT, GAME_FLAGS)) {
		return 0;
	}
	renderer = game_manager.engine.get_renderer();
	if (!renderer->initialize()) {
		return 0;
	}

	// Initialize modules.
	game_manager.client.on_initialize();
	return 1;
}

// Shut down the game manager.
void game_manager_shutdown(void)
{
	// Shut down modules.
	game_manager.client.on_shutdown();
}

// Run game manager for this tick.
int game_manager_tick(float delta_time)
{
	(void)delta_time;

	// Let modules prepare for tick.
	game_manager.client.on_tick_begin();
	
	// Run this tick.
	game_manager.client.on_tick();

	// Let modules clean up for next frame.
	game_manager.client.on_tick_end();
	return 1;
}

// Get current game time.
float game_manager_get_time(void)
{
	return 0.0f;
}

// Get a reference to the renderer interface.
const renderer_t *game_manager_get_renderer(void)
{
	return game_manager.engine.get_renderer();
}

// Get a reference to the renderer shader utilities.
const renderer_shader_utilities_t *game_manager_get_shader_utilities(void)
{
	return game_manager.engine.get_shader_utilities();
}

// Frame end callback for module to call when it's finished drawing.
void game_manager_present_frame(void)
{
	game_manager.engine.swap_buffers();
}
