#pragma once

#include "shared.h"

namespace Renderer
{

	// Interface for managing a texture resource.
	class Texture
	{

	public:

		// Destroy this texture resource.
		virtual void Destroy() = 0;
	
	};

}