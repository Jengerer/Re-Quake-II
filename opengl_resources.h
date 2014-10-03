#pragma once

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "opengl_common.h"
#include "renderer_resources.h"
#include "vector3.h"
#include "vector4.h"

namespace OpenGL
{

	// Implementing class for loading renderer resources.
	class Resources : public Renderer::Resources
	{

	public:

		// Create a model from a set of vertices.
		virtual Renderer::Model *CreateModel(
			const void *vertexData,
			int vertexCount,
			const Renderer::ShaderSchema *schema);

		// Create a model from a set of vertices and the indices referencing them.
		virtual Renderer::IndexedModel *CreateIndexedModel(
			const void *vertexData,
			int vertexCount,
			const unsigned int *indexData,
			int indexCount,
			const Renderer::ShaderSchema *schema);

		// Destroy a model.
		virtual void DestroyModel(Renderer::Model *model);

		// Destroy an indexed model.
		virtual void DestroyIndexedModel(Renderer::IndexedModel *model);

		// Loading and compiling a single shader.
		virtual Renderer::Shader *CreateShader(const char *filename, Renderer::ShaderType type);

		// Destroy/delete a shader.
		virtual void DestroyShader(Renderer::Shader *shader);

		// Build a shader program.
		virtual Renderer::Program *CreateProgram(const Renderer::Shader *vertexShader, const Renderer::Shader *fragmentShader);

		// Destroy a shader program.
		virtual void DestroyProgram(Renderer::Program *program);

		// Prepare a shader schema for vertex attributes.
		virtual Renderer::ShaderSchema *CreateShaderSchema(
			const Renderer::Program *program,
			const Renderer::Attribute *attributes,
			int attributeCount);

		// Destroy a shader schema.
		virtual void DestroyShaderSchema(Renderer::ShaderSchema *schema);

		// Get uniform variable from a program.
		virtual Renderer::Uniform *GetUniform(const Renderer::Program *program, const char *name);

		// Destroy uniform variable.
		virtual void DestroyUniform(Renderer::Uniform *uniform);

	};

}