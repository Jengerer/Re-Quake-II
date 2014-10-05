#pragma once

#include "engine_listener.h"
#include "game_manager_utilities.h"
#include "shared_defines.h"	

// Get the interface for engine to communicate with game manager.
LibraryExport Engine::Listener *GetEngineListener();