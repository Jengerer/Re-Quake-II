#include "client_export.h"
#include "game_client.h"

// Instantiate singleton instance.
Client Client::instance;

// Get structure for game manager to fill with utilities.
GameManager::Listener *GetClientGameManagerListener()
{
	return Client::GetInstance();
}
