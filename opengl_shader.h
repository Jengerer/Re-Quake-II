#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// OpenGL shader object.
	class Shader : public Renderer::Shader
	{

	public:

		Shader();
		~Shader();

		// Initialize shader from file.
		bool Initialize(const char *sourceFile, Renderer::ShaderType type);

		// Binding the texture 
		inline GLuint GetHandle() const;

	private:

		// Type conversion helper.
		static GLenum TranslateShaderType(Renderer::ShaderType type);

	private:

		GLuint handle;

	};

	// Get the OpenGL shader handle.
	GLuint Shader::GetHandle() const
	{
		return handle;
	}

}