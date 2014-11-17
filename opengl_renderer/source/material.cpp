#include "error_stack.h"
#include "memory_manager.h"
#include "material.h"
#include "material_layout.h"
#include "variable.h"

namespace OpenGL
{

	// Program nulling constructor.
	Material::Material() : handle(0), vertexShader(0), pixelShader(0)
	{
	}

	// Build the program for a pair of shaders.
	bool Material::Initialize(
		const char *vertexSource,
		int vertexSourceLength,
		const char *pixelSource,
		int pixelSourceLength)
	{
		// Create vertex shader and set reference for clean-up.
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		this->vertexShader = vertexShader;
		if (!Compile(vertexShader, vertexSource, vertexSourceLength)) {
			return false;
		}

		// Create pixel shader and set reference for clean-up.
		GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		this->pixelShader = pixelShader;
		if (!Compile(pixelShader, pixelSource, pixelSourceLength)) {
			return false;
		}

		// Create the program.
		GLuint handle = glCreateProgram();
		if (handle == 0) {
			ErrorStack::Log("Failed to create OpenGL program handle.");
			return false;
		}
		this->handle = handle;

		// Link the shaders and compile.
		glAttachShader(handle, vertexShader);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to attach vertex shader to program.");
			return false;
		}
		glAttachShader(handle, pixelShader);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to attach pixel shader to program.");
			return false;
		}

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
			ErrorStack::Log("Failed to link shader program.");
			return false;
		}
		return true;
	}

	// Destroy the material.
	void Material::Destroy()
	{
		MemoryManager::Destroy(this);
	}

	// Get a reference to a variable in this material.
	Renderer::Variable *Material::GetVariable(const char *name)
	{
		GLint location = glGetUniformLocation(handle, name);
		if (location == -1) {
			ErrorStack::Log("Cannot find material variable with name '%s'.", name);
			return nullptr;
		}

		// Create variable object.
		Variable *variable;
		if (!MemoryManager::Allocate(&variable)) {
			ErrorStack::Log("Failed to allocate variable object.");
			return nullptr;
		}
		new (variable) Variable(location);
		return static_cast<Renderer::Variable*>(variable);
	}

	// Get a matching material layout for this material.
	Renderer::MaterialLayout *Material::GetLayout(
		const Renderer::BufferLayout *bufferLayouts,
		int bufferCount)
	{
		// Create the material layout object.
		MaterialLayout *layout;
		if (!MemoryManager::Allocate(&layout)) {
			ErrorStack::Log("Failed to allocate material layout object.");
			return nullptr;
		}
		new (layout) MaterialLayout();
		
		// Convert the buffer layouts to OpenGL ones.
		BufferLayout *outLayout;
		if (!MemoryManager::AllocateArray(&outLayout, bufferCount)) {
			MemoryManager::Destroy(layout);
			ErrorStack::Log("Failed to allocate buffer layout objects.");
			return nullptr;
		}
		layout->SetBufferLayouts(outLayout, bufferCount);
		if (!layout->Initialize()) {
			MemoryManager::Destroy(layout);
			return nullptr;
		}

		// Initialize each buffer layout.
		for (int i = 0; i < bufferCount; ++i, ++outLayout, ++bufferLayouts) {
			// Create attribute array for this buffer.
			int attributeCount = bufferLayouts->GetAttributeCount();
			Attribute *attributes;
			if (!MemoryManager::AllocateArray(&attributes, attributeCount)) {
				MemoryManager::Destroy(layout);
				ErrorStack::Log("Failed to allocate attribute array.");
				return nullptr;
			}
			outLayout->SetAttributes(attributes, attributeCount);

			// Now fill in the parameters for each attribute.
			GLchar *offset = 0;
			for (int j = 0; j < attributeCount; ++j, ++attributes) {
				const Renderer::Attribute *current = bufferLayouts->GetAttribute(j);
				const GLchar *name = static_cast<const GLchar*>(current->GetName());
				Renderer::DataType dataType = current->GetDataType();

				// Get the location of the attribute.
				GLint location = glGetAttribLocation(handle, name);
				if (location != -1) {
					MemoryManager::Destroy(layout);
					ErrorStack::Log("Failed to get location of attribute '%s'.", name);
					return nullptr;
				}

				// Assign the parameters.
				offset = attributes->SetParameters(location, offset, dataType);
			}
		}
		return layout;
	}

	// Set this program to be used for rendering.
	void Material::Activate()
	{
		glUseProgram(handle);
	}

	// Unset this program from being used for rendering.
	void Material::Deactivate()
	{
		glUseProgram(0);
	}

	// Destroy the program (linked shaders are automatically unlinked).
	Material::~Material()
	{
		// Free the handle if we have one.
		if (handle != 0) {
			glDeleteProgram(handle);
		}
	}

	// Compile a shader.
	bool Material::Compile(GLuint shader, const char *source, int sourceLength)
	{
		glShaderSource(
			shader,
			1,
			static_cast<const GLchar**>(&source),
			static_cast<const GLint*>(&sourceLength));
		glCompileShader(shader);

		// Check for any complaints.
		GLchar *log;
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 1) {
			if (MemoryManager::AllocateArray(&log, logLength)) {
				glGetShaderInfoLog(shader, logLength, &logLength, log);
				// TODO: Print the warning/error.
				MemoryManager::DestroyArray(log);
			}
		}

		// Check for compilation success.
		GLint compileStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			return false;
		}
		return true;
	}

}