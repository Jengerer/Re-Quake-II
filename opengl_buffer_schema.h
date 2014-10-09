#pragma once

#include "opengl_common.h"
#include "opengl_program.h"
#include "opengl_shader_attribute.h"

namespace OpenGL
{

	// Structure for representing an OpenGL buffer schema.
	class BufferSchema : public Renderer::BufferSchema
	{

	public:

		BufferSchema();
		~BufferSchema();

		// Initialize schema from a set of renderer attributes.
		bool Initialize(
			const Program *program,
			const Renderer::Attribute *attributes,
			int attributeCount);

		// Enable this schema for the current render.
		void Activate() const;

		// Disable this schema from rendering.
		void Deactivate() const;

	private:

		ShaderAttribute *attributes;
		int attributeCount;

	};

}