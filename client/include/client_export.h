#pragma once

#include "define.h"
#include "game_module_interface.h"

// TODO: Combine server and client export once they're actually separate modules.
// Request the game module to fill out the game manager listener structure.
LibraryExport GameManager::ModuleInterface *GetClientInterface();