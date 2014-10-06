#include "error_stack.h"
#include "memory_manager.h"
#include "opengl_program.h"
#include "opengl_shader.h"

namespace OpenGL
{

	// Program nulling constructor.
	Program::Program() : handle(0)
	{
	}

	// Destroy the program (linked shaders are automatically unlinked).
	Program::~Program()
	{
		// Free the handle if we have one.
		if (handle != 0) {
			glDeleteProgram(handle);
		}
	}

	// Build the program for a pair of shaders.
	bool Program::Initialize(const Renderer::Shader *vertex, const Renderer::Shader *fragment)
	{
		// Create the program.
		GLuint handle = glCreateProgram();
		if (handle == 0) {
			ErrorStack::Log("Failed to create OpenGL program handle.");
			return false;
		}
		this->handle = handle;

		// Attach the vertex shader.
		const Shader *glShader = static_cast<const Shader*>(vertex);
		glAttachShader(handle, glShader->GetHandle());

		// Attach the fragment shader.
		glShader = static_cast<const Shader*>(fragment);
		glAttachShader(handle, glShader->GetHandle());

		// Build and check errors/warnings.
		GLint logLength;
		glLinkProgram(handle);
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength != 0) {
			GLchar *log;
			if (MemoryManager::AllocateArray(&log, logLength)) {
				glGetProgramInfoLog(handle, logLength, &logLength, log);
				// TODO: print to warning handler.
				MemoryManager::Free(log);
			}
		}
		GLint linkStatus;
		glGetProgramiv(handle, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			ErrorStack::Log("Failed to link shader program.\n");
			return false;
		}
		return true;
	}

	// Set this program to be used for rendering.
	void Program::Activate() const
	{
		glUseProgram(handle);
	}

	// Unset this program from being used for rendering.
	void Program::Deactivate() const
	{
		glUseProgram(0);
	}

	// Get location of a uniform variable within this program.
	GLint Program::GetUniformLocation(const char *name) const
	{
		return glGetUniformLocation(handle, name);
	}

}