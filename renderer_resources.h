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

		// Create a model from a set of vertices.
		virtual Model *CreateModel(
			const void *vertexData,
			int numVertices,
			const ShaderSchema *schema) = 0;

		// Create a model from a set of vertices and the indices referencing them.
		virtual Model *CreateIndexedModel(
			const void *vertexData,
			int numVertices,
			const unsigned int *indexData,
			int numIndices,
			const ShaderSchema *schema) = 0;

		// Destroy a model.
		virtual void DestroyModel(Model *model) = 0;

		// Loading and compiling a single shader.
		virtual Shader *CreateShader(const char *filename, ShaderType type) = 0;

		// Destroy/delete a shader.
		virtual void DestroyShader(Shader *shader) = 0;

		// Build a shader program.
		virtual Program *CreateProgram(const Shader *vertexShader, const Shader *fragmentShader) = 0;

		// Destroy a shader program.
		virtual void DestroyProgram(Program *program) = 0;

		// Prepare a shader schema for vertex attributes.
		virtual ShaderSchema *CreateShaderSchema(
			const Program *program,
			const Attribute *attributes,
			int numAttributes) = 0;

		// Destroy a shader schema.
		virtual void DestroyShaderSchema(ShaderSchema *schema) = 0;

		// Get uniform variable from a program.
		virtual Uniform *GetUniform(const Program *program, const char *name) = 0;

		// Destroy uniform variable.
		virtual void DestroyUniform(Uniform *uniform) = 0;

	};

}