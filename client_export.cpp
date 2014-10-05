#include "client_export.h"
#include "game_client.h"

// Instantiate singleton instance.
Client Client::instance;

// Get interface for game manager to pass events to.
GameManager::ModuleInterface *GetClientInterface()
{
	return static_cast<GameManager::ModuleInterface*>(Client::GetInstance());
}
