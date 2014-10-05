#pragma once

#include "shared_defines.h"
#include "game_manager_listener.h"
#include "game_manager_utilities.h"

// TODO: Combine server and client export once they're actually separate modules.
// Request the game module to fill out the game manager listener structure.
LibraryExport GameManager::Listener *GetClientGameManagerListener();