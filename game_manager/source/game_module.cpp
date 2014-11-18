#include "game_module.h"

GameModule::GameModule() : utilities(nullptr)
{
}

// Set the utilities interface.
void GameModule::SetGameManagerUtilities(GameManager::Utilities *utilities)
{
	this->utilities = utilities;
}

// Get game manager listener interface.
GameManager::Listener *GameModule::GetGameManagerListener()
{
	return static_cast<GameManager::Listener*>(this);
}