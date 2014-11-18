#pragma once

#include "client_define.h"
#include "game_module_interface.h"

// Get the interface for interacting with the client library.
ClientLibrary GameManager::ModuleInterface *GetClientInterface();