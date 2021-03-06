#pragma once

#include "game_manager_define.h"
#include "game_manager_shared.h"
#include <matrix3x3.h>
#include <matrix4x4.h>
#include <renderer/renderer_interface.h>
#include <renderer/resources_interface.h>
#include <renderer/shared.h>
#include <vector3.h>
#include <vector4.h>

namespace GameManager
{

	// Interface for module to access game assets.
	class GameManagerLibrary Utilities
	{

	public:

		// Request current game time.
		virtual float GetTime() const = 0;

		// Get window dimensions.
		virtual void GetWindowSize(int *width, int *height) = 0;

		// Get mouse position.
		virtual void GetMousePosition(int *x, int *y) = 0;

		// Set mouse position.
		virtual void SetMousePosition(int x, int y) = 0;

		// Swap buffer and present frame.
		virtual void PresentFrame() = 0;

		// Get renderer reference.
		virtual Renderer::Interface *GetRenderer() = 0;

		// Get renderer resources reference.
		virtual Renderer::Resources *GetRendererResources() = 0;

	};

}
