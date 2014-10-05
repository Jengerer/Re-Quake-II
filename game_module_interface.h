#pragma once

#include "game_manager_listener.h"
#include "game_manager_utilities.h"

namespace GameManager
{

	// Base game module for game manager to interact with.
	class ModuleInterface
	{

	public:

		// Get game manager listener interface.
		virtual GameManager::Listener *GetGameManagerListener() = 0;

	};

}