#pragma once

#include <GL/glew.h>
#include "renderer_shared.h"

namespace OpenGL
{
 
	// Convert renderer model type to OpenGL type.
	GLenum TranslateModelType(Renderer::ModelType modelType);

}