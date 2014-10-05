#pragma once

#include "game_manager_listener.h"
#include "game_manager_utilities.h"

namespace GameManager
{

	// Base game module for game manager to interact with.
	class Module : public GameManager::Listener
	{

	public:

		// Set interface through which modules get game manager resources.
		static void SetUtilities(GameManager::Utilities *utilities);

	private:

		// Game manager interface reference.
		GameManager::Utilities *utilities;

	};

}