#include "error_stack.h"
#include "memory_manager.h"
#include "opengl_shader_attribute.h"
#include "opengl_buffer_schema.h"

namespace OpenGL
{

	// Shader schema constructor.
	BufferSchema::BufferSchema() : attributes(nullptr)
	{
	}

	// Destroy shader schema and its attributes.
	BufferSchema::~BufferSchema()
	{
		// Destroy attributes if there are any.
		if (attributes != nullptr) {
			MemoryManager::Free(attributes);
		}
	}

	// Initialize schema from a set of renderer attributes.
	// Assumes the program is activated.
	bool BufferSchema::Initialize(
		const Program *program,
		const Renderer::Attribute *attributes,
		int attributeCount)
	{
		// Allocate space for OpenGL attributes.
		ShaderAttribute *glAttributes;
		if (!MemoryManager::AllocateArray(&glAttributes, attributeCount)) {
			ErrorStack::Log("Failed to allocate buffer for attribute array.");
			return false;
		}
		this->attributes = glAttributes;

		// Copy attribute data to GL-specific.
		GLchar *offset = 0;
		for (int i = 0; i < attributeCount; ++i, ++attributes, ++glAttributes) {
			ShaderAttribute *current = &glAttributes[i];

			// Get attribute parameters and initialize it.
			const char *name = attributes->GetName();
			GLint location = program->GetUniformLocation(name);
			if (location == -1) {
				ErrorStack::Log("Failed to get location of uniform variable: %s", name);
				return false;
			}

			// Construct with attribute parameters.
			Renderer::DataType type = attributes->GetType();
			new (current) ShaderAttribute(location, offset, type);

			// Go to next element.
			offset += ShaderAttribute::GetDataTypeSize(type);
		}

		// Fill in this schema's parameters.
		this->attributes = glAttributes;
		this->attributeCount = attributeCount;
		return true;
	}

	// Set this shader schema to be used for rendering.
	void BufferSchema::Activate() const
	{
		// Activate each vertex attribute.
		const ShaderAttribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Activate();
		}
	}

	// Unset this shader schema from rendering.
	void BufferSchema::Deactivate() const
	{
		// Deactivate each vertex attribute.
		const ShaderAttribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Deactivate();
		}
	}

}