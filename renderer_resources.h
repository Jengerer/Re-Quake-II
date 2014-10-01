#pragma once

#include "renderer_shared.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

// Interface for preparing shaders and uniform variables.
class RendererResources
{

public:

// Model functions.

	// Create a model from a set of vertices.
	virtual RendererModel *CreateModel(
		const void *vertexData,
		int numVertices,
		const RendererShaderSchema *schema);
	// Create a model from a set of vertices and the indices referencing them.
	virtual RendererModel *CreateIndexedModel(
		const void *vertexData,
		int numVertices,
		const unsigned int *indexData,
		int numIndices,
		const RendererShaderSchema *schema);
	// Destroy a model.
	virtual void DestroyModel(RendererModel *model);

// Texture functions.

	// Create a texture from an image.


// Shader functions.

	// Loading and compiling a single shader.
	virtual RendererShader *CreateShader(const char *filename, RendererShaderType type) = 0;
	// Destroy/delete a shader.
	virtual void DestroyShader(RendererShader *shader) = 0;
	// Build a shader program.
	virtual RendererProgram *CreateProgram(const RendererShader *vertexShader, const RendererShader *fragmentShader) = 0;
	// Destroy a shader program.
	virtual void DestroyProgram(RendererProgram *program) = 0;
	// Prepare a shader schema for vertex attributes.
	virtual RendererShaderSchema *CreateShaderSchema(
		const RendererProgram *program,
		const RendererShaderAttribute *attributes,
		int numAttributes) = 0;
	// Destroy a shader schema.
	virtual void DestroyShaderSchema(RendererShaderSchema *schema) = 0;

// Uniform variable functions.

	// Get uniform variable from a program.
	virtual RendererUniform *GetUniform(const RendererProgram *program, const char *name) = 0;
	// Destroy uniform variable.
	virtual void DestroyUniform(RendererUniform *uniform) = 0;

};