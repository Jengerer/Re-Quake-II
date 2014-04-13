#include "game.h"
#include <stdlib.h>

/*
 * Null the game context for safe destruction.
 */
void null_game(game_t *game)
{
	game->context = NULL;
}
