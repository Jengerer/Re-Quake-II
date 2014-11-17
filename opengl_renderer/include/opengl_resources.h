#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "opengl_common.h"
#include "renderer/resources.h"
#include "vector3.h"
#include "vector4.h"

namespace OpenGL
{

	// Implementing class for loading renderer resources.
	class Resources : public Renderer::Resources
	{

	public:

		// Loading and compiling a single shader.
		virtual Renderer::Shader *CreateShader(const char *filename, Renderer::ShaderType type);

		// Destroy/delete a shader.
		virtual void DestroyShader(Renderer::Shader *shader);

		// Build a shader program.
		virtual Renderer::Program *CreateProgram(const Renderer::Shader *vertexShader, const Renderer::Shader *fragmentShader);

		// Destroy a shader program.
		virtual void DestroyProgram(Renderer::Program *program);

		// Prepare a schema for passing data to shader.
		virtual Renderer::BufferSchema *CreateBufferSchema(
			const Renderer::Program *program,
			const Renderer::Attribute *attributes,
			int attributeCount);

		// Destroy a buffer schema.
		virtual void DestroyBufferSchema(Renderer::BufferSchema *schema);

		// Generate a buffer from a set of data.
		virtual Renderer::Buffer *CreateBuffer(
			const void *data,
			int bufferSize);

		// Destroy a buffer.
		virtual void DestroyBuffer(Renderer::Buffer *buffer);

		// Create an index buffer for referencing buffer data.
		virtual Renderer::IndexBuffer *CreateIndexBuffer(
			const void *indices,
			int bufferSize,
			int indexCount,
			Renderer::DataType type);

		// Destroy an index buffer.
		virtual void DestroyIndexBuffer(Renderer::IndexBuffer *buffer);

		// Get uniform variable from a program.
		virtual Renderer::Uniform *GetUniform(const Renderer::Program *program, const char *name);

		// Destroy uniform variable.
		virtual void DestroyUniform(Renderer::Uniform *uniform);

	public:

		// Singleton getter.
		static Resources *GetInstance();

	private:

		// Singleton instance.
		static Resources instance;

	};

}
