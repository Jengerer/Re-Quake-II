#pragma once

#include "shared.h"

namespace Renderer
{

	// Renderer drawing interface.
	class Interface
	{

	public:

		// Initialize renderer.
		virtual bool Initialize() = 0;

		// Shut down renderer.
		virtual void Destroy() = 0;

		// Clear the scene for a new frame.
		virtual void ClearScene() = 0;

	};

}