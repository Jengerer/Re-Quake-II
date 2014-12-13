#pragma once

#include "sdl_window.h"
#include "engine_interface.h"
#include "engine_listener.h"
#include "engine_utilities.h"
#include <renderer/renderer_interface.h>
#include <renderer/resources_interface.h>

namespace Engine
{

	// Engine implementation.
	class Implementation : public Interface, public Utilities
	{

	public:

		Implementation();
		~Implementation();

		// Get engine utilities interface.
		virtual Utilities *GetUtilities();

		// Engine interface implementation.
		virtual void SetRendererInterfaces(Renderer::Interface *renderer, Renderer::Resources *resources);
		virtual void SetListeners(Engine::Listener *engineListener, InputListener *inputListener);
		virtual bool Initialize();
		virtual void Shutdown();
		virtual bool Run();

		// Engine utilities implementation.
		virtual bool MakeWindow(const char *title, int width, int height, WindowFlags flags);
		virtual bool ResizeWindow(int width, int height);
		virtual bool UpdateWindowFlags(WindowFlags flags);
		virtual void SwapBuffers();
		virtual Renderer::Interface *GetRenderer();
		virtual Renderer::Resources *GetRendererResources();

		// Get singleton.
		static Implementation *GetInstance();

	private:

		// Singleton instance.
		static Implementation instance;

	private:

		SDLWindow window;
		Renderer::Interface *renderer;
		Renderer::Resources *rendererResources;
		Listener *listener;

	};

}