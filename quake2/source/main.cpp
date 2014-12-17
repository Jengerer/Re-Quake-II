#include <client_export.h>
#include <engine_export.h>
#include <error_stack.h>
#include <game_manager_export.h>
#include <memory_manager.h>
#include <opengl_export.h>
#include <renderer/attribute.h>
#include <renderer/buffer_layout.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// MemoryManager::SetBreakAllocation(11);

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

	engine->SetRendererInterfaces(renderer, resources);
	engine->SetListeners(engineListener, inputListener);
	gameManager->SetListeners(clientListener, nullptr);

	ErrorStack::Initialize();
	bool success = engine->Initialize() && engine->Run();
	if (!success) {
		ErrorStack::Dump();

#if defined(_DEBUG)
		system("pause");
#endif
	}

	engine->Shutdown();
	return success;
}
