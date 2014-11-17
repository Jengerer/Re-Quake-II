#include "opengl_export.h"
#include "opengl_renderer.h"
#include "opengl_resources.h"

// Get renderer interface reference.
Renderer::Interface *GetRendererInterface()
{
	return OpenGL::Implementation::GetInstance();
}

// Get renderer resources interface.
Renderer::Resources *GetRendererResources()
{
	return OpenGL::Resources::GetInstance();
}
