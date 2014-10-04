#pragma once

namespace Engine
{

	// Interface through which the engine sends its events.
	class Listener
	{

	public:

		// Triggered when the engine is fully initialized.
		virtual bool OnInitialize() = 0;

		// Triggered when engine wants to exit.
		virtual void OnShutdown() = 0;

		// Called on every frame.
		virtual int OnTick(float timeDelta) = 0;

	};

}