#pragma once

#include "game_manager_define.h"
#include "game_manager_shared.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "renderer/renderer_interface.h"
#include "renderer/resources_interface.h"
#include "renderer/shared.h"
#include "vector3.h"
#include "vector4.h"

namespace GameManager
{

	// Interface for module to access game assets.
	class GameManagerLibrary Utilities
	{

	public:

		// Request current game time.
		virtual float GetTime() const = 0;

		// Swap buffer and present frame.
		virtual void PresentFrame() = 0;

	public:

		// Implementing utilities singleton.
		static Utilities *instance;

		// Renderer interface.
		static Renderer::Interface *renderer;

		// Renderer resource interface.
		static Renderer::Resources *resources;

	};

}
