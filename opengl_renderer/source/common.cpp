#include "common.h"
#include "error_stack.h"

namespace OpenGL
{

	// Convert primitive type to OpenGL enum.
	GLenum Common::TranslatePrimitiveType(Renderer::PrimitiveType type)
	{
		switch (type) {
		case Renderer::Triangles:
			return GL_TRIANGLES;
		case Renderer::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case Renderer::TriangleFan:
			return GL_TRIANGLE_FAN;
		default:
			ErrorStack::Log("Invalid primitive type translated.");
			return 0;
		}
	}
}
