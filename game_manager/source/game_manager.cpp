#include "game_manager.h"
#include "engine_interface.h"

// TODO: Erase these once we get settings.
const char *GameTitle = "Game Test";
const int GameWidth = 1024;
const int GameHeight = 768;

namespace GameManager
{

	// Singleton instance.
	Implementation Implementation::instance;

	// Clearing game manager constructor.
	Implementation::Implementation()
		: engineUtilities(nullptr)
	{
	}

	// Destroy game manager.
	Implementation::~Implementation()
	{
	}

	// Get interface for engine to pass events to.
	Engine::Listener *Implementation::GetEngineListener()
	{
		return static_cast<Engine::Listener*>(this);
	}

	// Get interface for engine to pass input events to.
	InputListener *Implementation::GetInputListener()
	{
		return static_cast<InputListener*>(this);
	}

	// Set client and server module listeners.
	void Implementation::SetListeners(GameManager::Listener *client, GameManager::Listener *server)
	{
		(void)server;
		this->clientListener = client;
	}

	// Engine is set up.
	bool Implementation::OnInitialize(Engine::Utilities *engineUtilities)
	{
		// Save the upper utilities interface.
		this->engineUtilities = engineUtilities;

		// Set up renderer and create window.
		WindowFlags flags;
		flags.raw = 0;
		if (!engineUtilities->MakeWindow(GameTitle, GameWidth, GameHeight, flags)) {
			return false;
		}

		// Pass to client to set up.
		if (!clientListener->OnInitialized(this)) {
			return false;
		}
		return true;
	}

	// Engine is shutting down, clean up.
	void Implementation::OnShutdown()
	{
		clientListener->OnShutdown();
	}

	// Engine frame update.
	bool Implementation::OnTick(float timeDelta)
	{
		(float)timeDelta;
		if (!clientListener->OnTickBegin()) {
			return false;
		}
		if (!clientListener->OnTick()) {
			return false;
		}
		if (!clientListener->OnTickEnd()) {
			return false;
		}
		return true;
	}

	// Handle key press event.
	InputEventResult Implementation::OnKeyPress(KeyCode key)
	{
		(void)key;
		return InputEventSuccess;
	}

	// Handle key release event.
	InputEventResult Implementation::OnKeyRelease(KeyCode key)
	{
		(void)key;
		return InputEventSuccess;
	}

	// Get current game time.
	float Implementation::GetTime() const
	{
		return 0.0f;
	}

	// Swap buffers and show current render.
	void Implementation::PresentFrame()
	{
		engineUtilities->SwapBuffers();
	}

	// Get renderer interface reference.
	Renderer::Interface *Implementation::GetRenderer()
	{
		return engineUtilities->GetRenderer();
	}

	// Get renderer resources interface reference.
	Renderer::Resources *Implementation::GetRendererResources()
	{
		return engineUtilities->GetRendererResources();
	}

	// Get singleton instance.
	Implementation *Implementation::GetInstance()
	{
		return &instance;
	}

}
