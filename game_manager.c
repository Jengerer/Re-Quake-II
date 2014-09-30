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