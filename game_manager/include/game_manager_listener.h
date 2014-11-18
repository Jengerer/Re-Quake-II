#pragma once

#include "game_manager_utilities.h"

namespace GameManager
{

	// Class for game manager event receiver.
	class Listener
	{

	public:

		// Game manager initialized.
		virtual bool OnInitialized(Utilities *utilities) = 0;

		// Game manager about to be shut down.
		virtual void OnShutdown() = 0;

		// Start of game manager tick.
		virtual bool OnTickBegin() = 0;

		// All start tick functions finished.
		virtual bool OnTick() = 0;

		// End of game manager tick.
		virtual bool OnTickEnd() = 0;

	};

}