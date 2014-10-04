#include "engine_export.h"
#include "engine.h"

// Get the singleton instance.
Engine::Interface *GetEngineInterface()
{
	return static_cast<Engine::Interface*>(Engine::Implementation::GetInstance());
}
