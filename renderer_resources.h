#pragma once

#include "renderer_shared.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace Renderer
{

	// Interface for preparing shaders and uniform variables.
	class Resources
	{

	public:

		// Loading and compiling a single shader.
		virtual Shader *CreateShader(const char *filename, ShaderType type) = 0;

		// Destroy/delete a shader.
		virtual void DestroyShader(Shader *shader) = 0;

		// Build a shader program.
		virtual Program *CreateProgram(const Shader *vertexShader, const Shader *fragmentShader) = 0;

		// Destroy a shader program.
		virtual void DestroyProgram(Program *program) = 0;

		// Prepare a schema for a buffer to map data to shader attributes.
		virtual BufferSchema *CreateBufferSchema(
			const Program *program,
			const Attribute *attributes,
			int attributeCount) = 0;

		// Destroy a buffer schema.
		virtual void DestroyBufferSchema(BufferSchema *schema) = 0;

		// Generate a buffer from a set of data.
		virtual Buffer *CreateBuffer(const void *data, int bufferSize, const BufferSchema *schema) = 0;

		// Destroy a buffer.
		virtual void DestroyBuffer(Buffer *buffer) = 0;

		// Generate an index buffer for referencing buffer data.
		virtual IndexBuffer *CreateIndexBuffer(const void *data, int bufferSize, int count, DataType type) = 0;

		// Destroy an index buffer.
		virtual void DestroyIndexBuffer(IndexBuffer *buffer) = 0;

		// Get uniform variable from a program.
		virtual Uniform *GetUniform(const Program *program, const char *name) = 0;

		// Destroy uniform variable.
		virtual void DestroyUniform(Uniform *uniform) = 0;

	};

}
