#pragma once

#include "opengl_common.h"
#include "opengl_program.h"
#include "opengl_shader_attribute.h"

namespace OpenGL
{

	// Structure for representing an OpenGL buffer schema.
	class ShaderSchema : public Renderer::BufferSchema
	{

	public:

		ShaderSchema();
		~ShaderSchema();

		// Initialize schema from a set of renderer attributes.
		bool Initialize(
			const Program *program,
			const Renderer::Attribute *attributes,
			int attributeCount);

		// Enable this schema for the current render.
		void Activate() const;

		// Disable this schema from rendering.
		void Deactivate() const;

		// Get vertex size.
		inline GLsizei GetVertexSize() const;

	private:

		// Helper for determining number of floats in a given type.
		static int GetVariableFloatCount(Renderer::VariableType type);

	private:

		GLsizei vertexSize;
		ShaderAttribute *attributes;
		int attributeCount;

	};

	// Retrieve total size of vertex defined by all attributes in this schema.
	GLsizei ShaderSchema::GetVertexSize() const
	{
		return vertexSize;
	}

}