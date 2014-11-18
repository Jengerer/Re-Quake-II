#pragma once

#include "game_manager_define.h"
#include "game_module_interface.h"

// Base game module.
class GameManagerLibrary GameModule : public GameManager::ModuleInterface, GameManager::Listener
{

public:

	GameModule();

	// Get game manager listener interface.
	virtual GameManager::Listener *GetGameManagerListener();

};