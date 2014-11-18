#include "error_stack.h"
#include "renderer/attribute.h"
#include "renderer/buffer_layout.h"
#include <stdio.h>
#include <stdlib.h>

// Sample Quake II buffer layout.
const int StartFrameAttributeCount = 2;
const int EndFrameAttributeCount = StartFrameAttributeCount;
const int TextureAttributeCount = 1;
const Renderer::Attribute startFrameAttributes[StartFrameAttributeCount] =
{
	Renderer::Attribute("position0", Renderer::PositionType, Renderer::Vector3Type),
	Renderer::Attribute("normal0", Renderer::NormalType, Renderer::Vector3Type)
};
const Renderer::Attribute endFrameAttributes[EndFrameAttributeCount] = {
	Renderer::Attribute("position1", Renderer::PositionType, Renderer::Vector3Type),
	Renderer::Attribute("normal1", Renderer::NormalType, Renderer::Vector3Type)
};
const Renderer::Attribute textureAttributes[TextureAttributeCount] = {
	Renderer::Attribute("uv", Renderer::TextureCoordinateType, Renderer::Vector2Type)
};
const int QuakeBufferCount = 3;
const Renderer::BufferLayout QuakeBufferLayouts[QuakeBufferCount] = {
	Renderer::BufferLayout(startFrameAttributes, StartFrameAttributeCount),
	Renderer::BufferLayout(endFrameAttributes, EndFrameAttributeCount),
	Renderer::BufferLayout(textureAttributes, TextureAttributeCount)
};

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	bool success;


	/*
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
	*/
	return success;
}

