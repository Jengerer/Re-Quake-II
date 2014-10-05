#include "game_manager_export.h"
#include "game_manager.h"

// Get reference to game manager event handling interface.
GameManager::Interface *GetGameManagerInterface()
{
	return static_cast<GameManager::Interface*>(GameManager::Implementation::GetInstance());
}