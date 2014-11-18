#include "error_stack.h"
#include "file.h"
#include "material.h"
#include "material_layout.h"
#include "memory_manager.h"
#include "renderer.h"

namespace OpenGL
{

	// Singleton instance instantiation.
	Implementation Implementation::instance;

	Implementation::Implementation()
	{
	}

	Implementation::~Implementation()
	{
	}

	// Initialize OpenGL interface.
	bool Implementation::Initialize()
	{
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			ErrorStack::Log("Unable to initialize GLEW: %s.\n", glewGetErrorString(error));
			return 0;
		}

		// Set up GL rendering parameters.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		return 1;
	}

	// Deallocate OpenGL.
	void Implementation::Destroy()
	{
	}

	// Clear scene for next frame.
	void Implementation::ClearScene()
	{
		// TODO: Make this an option for client.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Set material for rendering.
	void Implementation::SetMaterial(Renderer::Material *material)
	{
		Material *glMaterial = static_cast<Material*>(material);
		glMaterial->Activate();
	}

	// Unset material from rendering.
	void Implementation::UnsetMaterial(Renderer::Material *material)
	{
		Material *glMaterial = static_cast<Material*>(material);
		glMaterial->Deactivate();
	}

	// Activate a material layout.
	void Implementation::SetMaterialLayout(Renderer::MaterialLayout *layout)
	{
		MaterialLayout *glLayout = static_cast<MaterialLayout*>(layout);
		glLayout->Activate();
	}

	// Deactivate a material layout.
	void Implementation::UnsetMaterialLayout(Renderer::MaterialLayout *layout)
	{
		MaterialLayout *glLayout = static_cast<MaterialLayout*>(layout);
		glLayout->Deactivate();
	}

	// Get singleton instance of renderer.
	Implementation *Implementation::GetInstance()
	{
		return &instance;
	}

}

/*
// Bind a 2D texture to render.
void opengl_bind_texture2d(renderer_texture_t texture, renderer_uniform_t shader_texture)
{
	// Bind the texture.
	opengl_texture_t *opengl_texture = (opengl_texture_t*)texture.buffer;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, opengl_texture->handle);

	// Set the uniform value to match this texture slot.
	// TODO: Clean this up to properly use texture indices.
	opengl_set_uniform_integer(shader_texture, 0);
}


// Unbind the 2D texture.
void opengl_unbind_texture2d(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

*/