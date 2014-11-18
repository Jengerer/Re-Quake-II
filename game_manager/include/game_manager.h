#pragma once

#include "engine_listener.h"
#include "engine_utilities.h"
#include "game_manager_interface.h"
#include "game_manager_utilities.h"
#include "game_manager_listener.h"
#include "input_listener.h"

namespace GameManager
{

	// Implementing class for engine listener and game module manager.
	class Implementation
		: public Interface,
		public InputListener,
		public Utilities,
		public Engine::Listener
	{

		Listener *listener;

	public:

		Implementation();
		~Implementation();

		// Get listener to pass to engine.
		virtual Engine::Listener *GetEngineListener();

		// Get input listener to pass to engine.
		virtual InputListener *GetInputListener();

		// Pass client and server listeners.
		virtual void SetListeners(GameManager::Listener *client, GameManager::Listener *server);

		// Engine listener functions.
		virtual bool OnInitialize(Engine::Utilities *engineUtilities);
		virtual void OnShutdown();
		virtual bool OnTick(float timeDelta);

		// Input listener functions.
		virtual InputEventResult OnKeyPress(KeyCode key);
		virtual InputEventResult OnKeyRelease(KeyCode key);

		// Game manager utilities.
		virtual float GetTime() const;
		virtual void PresentFrame();

		// Singleton retriever.
		static Implementation *GetInstance();

	private:

		// Singleton instance.
		static Implementation instance;

		// Interface for retrieving assets from engine.
		Engine::Utilities *engineUtilities;

		// Game manager modules.
		GameManager::Listener *clientListener;

	};

}