#pragma once

#include "renderer.h"
#include "renderer_resources.h"
#include "engine_listener.h"

// Interface for an application to interact with the engine.
namespace Engine
{

	// Interface for an application to handle the engine.
	class Interface
	{

	public:

		// Pass in interfaces.
		virtual void SetRendererInterfaces(Renderer::Interface *renderer, Renderer::Resources *resources);
		virtual void SetEngineListener(Engine::Listener *listener);

		// Engine initialization.
		virtual bool Initialize() = 0;

		// Engine destruction.
		virtual void Shutdown() = 0;

		// Engine main loop.
		virtual bool Run() = 0;

	};

}