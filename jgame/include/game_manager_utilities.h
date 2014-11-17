#pragma once

#include "game_manager_shared.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "renderer_shared.h"
#include "vector3.h"
#include "vector4.h"

namespace GameManager
{

	// Interface for module to access game assets.
	class Utilities
	{

	public:

		// Request current game time.
		virtual float GetTime() const = 0;

		// Clear buffer for new frame.
		virtual void ClearScene() = 0;

		// Swap buffer and present frame.
		virtual void PresentFrame() = 0;

		// Allocate a graphics buffer for vertex/index data.
		virtual Renderer::Buffer *CreateBuffer(const void *data, int bufferSize, int elementSize) = 0;

		// Destroy a buffer.
		virtual void DestroyBuffer(Renderer::Buffer *buffer) = 0;

		// Create shader from file.
		virtual Renderer::Shader *CreateShader(const char *filename, Renderer::ShaderType type) = 0;

		// Destroy shader.
		virtual void DestroyShader(Renderer::Shader *shader) = 0;

		// Build vertex and fragment shaders into program.
		virtual Renderer::Program *CreateProgram(const Renderer::Shader *vertex, const Renderer::Shader *fragment) = 0;

		// Destroy shader program.
		virtual void DestroyProgram(Renderer::Program *program) = 0;

		// Set program for rendering.
		virtual void SetProgram(const Renderer::Program *program) = 0;

		// Unset program from rendering.
		virtual void UnsetProgram(const Renderer::Program *program) = 0;

		// Get uniform variable.
		virtual Renderer::Uniform *GetUniform(const Renderer::Program *program, const char *name) = 0;

		// Destroy uniform variable reference.
		virtual void DestroyUniform(Renderer::Uniform *uniform) = 0;

		// Set uniform variable 4x4 matrix value.
		virtual void SetUniform(const Renderer::Uniform *uniform, const Matrix4x4 *matrix) = 0;

	};

}
