#include "client_export.h"
#include "engine_export.h"
#include "error_stack.h"
#include "game_manager_export.h"
#include "memory_manager.h"
#include "opengl_export.h"
#include "renderer/attribute.h"
#include "renderer/buffer_layout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	bool success;

	// Allocation debugging.
	// MemoryManager::SetBreakAllocation(3);

	// Get engine implementation.
	Engine::Interface *engine = GetEngineInterface();

	// Get renderer implementation.
	Renderer::Interface *renderer = GetRendererInterface();
	Renderer::Resources *resources = GetRendererResources();

	// Get game manager implementation.
	GameManager::Interface *gameManager = GetGameManagerInterface();
	Engine::Listener *engineListener = gameManager->GetEngineListener();
	InputListener *inputListener = gameManager->GetInputListener();

	// Get client implementation.
	GameManager::ModuleInterface *client = GetClientInterface();
	GameManager::Listener *clientListener = client->GetGameManagerListener();

	// Set up engine.
	engine->SetRendererInterfaces(renderer, resources);
	engine->SetListeners(engineListener, inputListener);

	// Set up game manager.
	gameManager->SetListeners(clientListener, nullptr);

	// Initialize error stack for error logging..
	ErrorStack::Initialize();

	// Initialize engine and run.
	success = engine->Initialize() && engine->Run();

	// How did we do?
	if (!success) {
		// Dump what went wrong.
		ErrorStack::Dump();

		// Pause at the end so we can read errors.
#if defined(_DEBUG)
		system("pause");
#endif
	}

	engine->Shutdown();
	return success;
}

