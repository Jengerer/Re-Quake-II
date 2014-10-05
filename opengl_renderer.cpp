#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include "opengl_program.h"
#include "opengl_renderer.h"
#include "opengl_uniform.h"

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
	void Implementation::Shutdown()
	{
	}

	// Clear scene for next frame.
	void Implementation::ClearScene()
	{
		// TODO: Make this an option for client.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Set program as active.
	void Implementation::SetProgram(const Renderer::Program *program)
	{
		const Program *glProgram = static_cast<const Program*>(program);
		glProgram->Activate();
	}

	// Set program as active.
	void Implementation::UnsetProgram(const Renderer::Program *program)
	{
		const Program *glProgram = static_cast<const Program*>(program);
		glProgram->Deactivate();
	}

	// Set uniform value as float.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, float value)
	{
		const Uniform *glUniform = static_cast<const Uniform*>(uniform);
		glUniform->SetValue(value);
	}

	// Set value as 3-D vector.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, const Vector3 *vector)
	{
		const Uniform *glUniform = static_cast<const Uniform*>(uniform);
		glUniform->SetValue(vector);
	}

	// Set value as 4-D vector.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, const Vector4 *vector)
	{
		const Uniform *glUniform = static_cast<const Uniform*>(uniform);
		glUniform->SetValue(vector);
	}

	// Set value as 3x3 matrix.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, const Matrix3x3 *matrix)
	{
		const Uniform *glUniform = static_cast<const Uniform*>(uniform);
		glUniform->SetValue(matrix);
	}

	// Set value as 4x4 matrix.
	void Implementation::SetUniform(const Renderer::Uniform *uniform, const Matrix4x4 *matrix)
	{
		const Uniform *glUniform = static_cast<const Uniform*>(uniform);
		glUniform->SetValue(matrix);
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