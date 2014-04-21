#include "game.h"
#include <stdlib.h>

/*
 * Null the game context for safe destruction.
 */
void null_game(game_t *game)
{
	game->get_name = NULL;
	game->initialize = NULL;
	game->destroy = NULL;
	game->load_resources = NULL;
	game->free_resources = NULL;
	game->render = NULL;
	game->handle_keyboard = NULL;
}

