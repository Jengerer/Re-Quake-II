#pragma once

#include <GL/glew.h>
#include "renderer/shared.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace OpenGL
{

	// Generic common functions.
	class Common
	{

	public:

		// Convert primitive type to OpenGL primitive.
		static GLenum TranslatePrimitiveType(Renderer::PrimitiveType type);

	};

}