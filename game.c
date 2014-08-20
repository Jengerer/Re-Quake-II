#include "game.h"
#include <stdlib.h>

/*
 * Null the game context for safe destruction.
 */
void game_null(game_t *game)
{
	game->get_name = NULL;
	game->initialize = NULL;
	game->destroy = NULL;
	game->load_resources = NULL;
	game->free_resources = NULL;
	game->render = NULL;
	game->frame_begin = NULL;
	game->frame_end = NULL;
	game->handle_keyboard = NULL;
}
