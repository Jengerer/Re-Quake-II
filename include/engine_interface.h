#pragma once

#include "engine_listener.h"
#include "input_listener.h"
#include "renderer/renderer.h"
#include "renderer_resources.h"

// Interface for an application to interact with the engine.
namespace Engine
{

	// Interface for an application to handle the engine.
	class Interface
	{

	public:

		// Pass in interfaces.
		virtual void SetRendererInterfaces(Renderer::Interface *renderer, Renderer::Resources *resources) = 0;
		virtual void SetListeners(Engine::Listener *engineListener, InputListener *inputListener) = 0;

		// Engine initialization.
		virtual bool Initialize() = 0;

		// Engine destruction.
		virtual void Shutdown() = 0;

		// Engine main loop.
		virtual bool Run() = 0;

	};

}