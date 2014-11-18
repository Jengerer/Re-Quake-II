#include "game_module.h"

GameModule::GameModule()
{
}

// Get game manager listener interface.
GameManager::Listener *GameModule::GetGameManagerListener()
{
	return static_cast<GameManager::Listener*>(this);
}
