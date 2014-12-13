#include "engine.h"
#include "sdl_window.h"
#include <memory_manager.h>

namespace Engine
{

	// Singleton instance.
	Implementation Implementation::instance;

	Implementation::Implementation()
		: renderer(nullptr), rendererResources(nullptr), listener(nullptr)
	{
	}

	Implementation::~Implementation()
	{
	}

	// Get reference to engine utilities.
	Utilities *Implementation::GetUtilities()
	{
		return static_cast<Utilities*>(this);
	}

	// Update renderer interface references.
	void Implementation::SetRendererInterfaces(Renderer::Interface *renderer, Renderer::Resources *resources)
	{
		this->renderer = renderer;
		this->rendererResources = resources;
	}

	// Update engine module listeners.
	void Implementation::SetListeners(Engine::Listener *engineListener, InputListener *inputListener)
	{
		this->listener = engineListener;
		window.SetInputListener(inputListener);
	}

	// Initialize engine components.
	bool Implementation::Initialize()
	{
		// Don't initialize renderer yet; game manager should get to choose size/flags.
		MemoryManager::Initialize();
		if (!listener->OnInitialize(this)) {
			return false;
		}
		return true;
	}

	// Shut down components.
	void Implementation::Shutdown()
	{
		// Shut down listener first so he can clean renderer components.
		listener->OnShutdown();
		renderer->Destroy();
		MemoryManager::Shutdown();
	}

	// Enter main loop for engine.
	bool Implementation::Run()
	{
		WindowEventResult result;
		bool done = false;
		while (!done) {
			// Handle window events.
			result = window.HandleEvents();
			switch (result) {
			case WindowEventSuccess:
				break;
			case WindowEventError:
				return false;
			case WindowEventQuit:
				return true;
			}

			// Pass tick event to listener.
			if (!listener->OnTick(1.0f / 60.0f)) {
				return false;
			}
		}
		return true;
	}

	// Create a window and set up the renderer for it.
	// Assumes this function is only called once, and then only update functions are called hereafter.
	bool Implementation::MakeWindow(const char *title, int width, int height, WindowFlags flags)
	{
		if (!window.Create(title, width, height, flags)) {
			return false;
		}
		if (!renderer->Initialize()) {
			return false;
		}
		return true;
	}

	// Resize the window.
	bool Implementation::ResizeWindow(int width, int height)
	{
		window.ResizeWindow(width, height);
		return true;
	}

	// Update window flags.
	bool Implementation::UpdateWindowFlags(WindowFlags flags)
	{
		if (!window.UpdateFlags(flags)) {
			return false;
		}
		return true;
	}

	// Swap frame buffer.
	void Implementation::SwapBuffers()
	{
		window.SwapBuffers();
	}

	// Get reference to renderer.
	Renderer::Interface *Implementation::GetRenderer()
	{
		return renderer;
	}

	// Get reference to renderer resource interface.
	Renderer::Resources *Implementation::GetRendererResources()
	{
		return rendererResources;
	}

	// Get pointer to singleton instance.
	Implementation *Implementation::GetInstance()
	{
		return &instance;
	}

}
