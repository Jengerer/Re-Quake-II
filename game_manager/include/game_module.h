#pragma once

#include "game_module_interface.h"

// Base game module.
class GameModule : public GameManager::ModuleInterface, GameManager::Listener
{

public:

	GameModule();

	// Base game manager initialization function.
	void SetGameManagerUtilities(GameManager::Utilities *utilities);

	// Get game manager listener interface.
	virtual GameManager::Listener *GetGameManagerListener();

protected:

	GameManager::Utilities *utilities;

};