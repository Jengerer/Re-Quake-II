#include "error_stack.h"
#include "memory_manager.h"
#include "opengl_shader_schema.h"
#include "opengl_shader_attribute.h"
#include <new.h>

namespace OpenGL
{

	// Shader schema constructor.
	ShaderSchema::ShaderSchema() : attributes(nullptr)
	{
	}

	// Destroy shader schema and its attributes.
	ShaderSchema::~ShaderSchema()
	{
		// Destroy attributes if there are any.
		if (attributes != nullptr) {
			MemoryManager::Free(attributes);
		}
	}

	// Initialize schema from a set of renderer attributes.
	bool ShaderSchema::Initialize(
		const Program *program,
		const Renderer::Attribute *attributes,
		int attributeCount)
	{
		// Allocate space for OpenGL attributes.
		ShaderAttribute *glAttributes = reinterpret_cast<ShaderAttribute*>(MemoryManager::AllocateArray(sizeof(ShaderAttribute) * attributeCount));
		if (attributes == nullptr) {
			ErrorStack::Log("Failed to allocate buffer for attribute array.\n");
			return false;
		}

		// Copy attribute data to GL-specific.
		GLsizei vertexSize = 0;
		GLuint handle = program->GetHandle();
		for (int i = 0; i < attributeCount; ++i, ++attributes) {
			ShaderAttribute *current = &glAttributes[i];

			// Get attribute parameters and initialize it.
			GLint location = glGetAttribLocation(handle, attributes->GetName());
			int floatCount = GetVariableFloatCount(attributes->GetType());
			GLchar *offset = reinterpret_cast<GLchar*>(vertexSize);
			new (current) ShaderAttribute(location, floatCount, offset);

			// Increase vertex size.
			vertexSize += floatCount * sizeof(float);
		}

		// Fill in this schema's parameters.
		this->vertexSize = vertexSize;
		this->attributes = glAttributes;
		this->attributeCount = attributeCount;
		return true;
	}

	// Set this shader schema to be used for rendering.
	void ShaderSchema::Activate() const
	{
		// Activate each vertex attribute.
		const ShaderAttribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Activate(vertexSize);
		}
	}

	// Unset this shader schema from rendering.
	void ShaderSchema::Deactivate() const
	{
		// Deactivate each vertex attribute.
		const ShaderAttribute *attribute = attributes;
		for (int i = 0; i < attributeCount; ++i, ++attribute) {
			attribute->Deactivate();
		}
	}

	// Return how many floats fit perfectly into a given variable type.
	int ShaderSchema::GetVariableFloatCount(Renderer::VariableType type)
	{
		switch (type) {
		case Renderer::FloatType:
			return 1;
		case Renderer::Vector2Type:
			return 2;
		case Renderer::Vector3Type:
			return 3;
		case Renderer::Vector4Type:
			return 4;
		default:
			break;
		}

		// This should never be reached, but probably will, and will cause anguish.
		return 0;
	}

}