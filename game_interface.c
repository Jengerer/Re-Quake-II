#include "game_interface.h"
#include "memory_manager.h"

/*
 * Null the game context for safe destruction.
 */
void game_interface_null(game_interface_t *game)
{
	game->get_name = NULL;
	game->initialize = NULL;
	game->destroy = NULL;
	game->render = NULL;
	game->frame_begin = NULL;
	game->frame_end = NULL;
	game->handle_keyboard = NULL;
}
