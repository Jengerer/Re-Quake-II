#include "opengl_shader.h"
#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include <stdio.h>

namespace OpenGL
{

	// Clearing shader constructor.
	Shader::Shader() : handle(0)
	{
	}

	// Destroy the shader.
	Shader::~Shader()
	{
		// Clean up handle if we have one.
		if (handle != 0) {
			glDeleteShader(handle);
		}
	}

	// Shader loading and compiling.
	bool Shader::Initialize(const char *sourceFile, Renderer::ShaderType type)
	{
		// Load the source file.
		File file;
		if (!file.Read(sourceFile)) {
			ErrorStack::Log("Failed to load shader file: %s\n", sourceFile);
			return false;
		}

		// Create shader of the given type.
		GLenum glType = TranslateShaderType(type);
		GLuint handle = glCreateShader(glType);
		if (handle == 0) {
			ErrorStack::Log("Failed to create OpenGL shader handle.\n");
			return false;
		}
		this->handle = handle;

		// Load the source and compile.
		const GLchar *sourceText = reinterpret_cast<const GLchar*>(file.GetBuffer());
		GLint shaderLength = static_cast<GLint>(file.GetSize());
		glShaderSource(handle, 1, &sourceText, &shaderLength);
		glCompileShader(handle);

		// Check for warnings.
		GLint logLength;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength != 0) {
			GLchar *log;
			if (MemoryManager::AllocateArray(&log, logLength)) {
				glGetShaderInfoLog(handle, logLength, &logLength, log);
				// TODO: log to warning handler.
				printf("Output for compilation of %s:\n%s\n", sourceFile, log);
				MemoryManager::Free(log);
			}
		}

		// Check for compilation success.
		GLint compileStatus;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			ErrorStack::Log("Failed to compile shader: %s\n", sourceFile);
			return false;
		}
		return true;
	}

	// Translate generic renderer shader type to OpenGL enum.
	GLenum Shader::TranslateShaderType(Renderer::ShaderType type)
	{
		switch (type) {
		case Renderer::VertexShader:
			return GL_VERTEX_SHADER;
		case Renderer::FragmentShader:
			return GL_FRAGMENT_SHADER;
		}
		return 0;
	}

}