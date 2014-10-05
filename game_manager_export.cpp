#include "game_manager_export.h"
#include "game_manager.h"

// Get reference to game manager event handling interface.
Engine::Listener *GetEngineListener()
{
	return static_cast<Engine::Listener*>(GameManager::Implementation::GetInstance());
}