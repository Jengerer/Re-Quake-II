#include "opengl_common.h"

namespace OpenGL
{

	// Convert model type from renderer type.
	GLenum TranslateModelType(Renderer::ModelType modelType)
	{
		switch (modelType) {
		case Renderer::TriangleStripModel:
			return GL_TRIANGLE_STRIP;
		case Renderer::TriangleFanModel:
			return GL_TRIANGLE_FAN;
		default:
			return 0;
		}
	}

}