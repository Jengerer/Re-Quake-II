#pragma once

#include "engine_listener.h"
#include "engine_utilities.h"
#include "game_manager_utilities.h"
#include "game_manager_listener.h"
#include "input_listener.h"

namespace GameManager
{

	// Interface for passing listeners to the game manager.
	class Interface
	{

	public:

		// Get engine listener interface.
		virtual Engine::Listener *GetEngineListener() = 0;

		// Get input listener interface.
		virtual InputListener *GetInputListener() = 0;

		// Set client and server listener interfaces.
		virtual void SetListeners(GameManager::Listener *client, GameManager::Listener *server) = 0;

	};

}