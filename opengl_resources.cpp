#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include "opengl_indexed_model.h"
#include "opengl_model.h"
#include "opengl_program.h"
#include "opengl_resources.h"
#include "opengl_shader.h"
#include "opengl_shader_schema.h"
#include "opengl_uniform.h"
#include "shared_defines.h"
#include <new.h>

namespace OpenGL
{

	// Singleton instance instantiation.
	Resources Resources::instance;

	// Create a model from a set of vertex data.
	Renderer::Model *Resources::CreateModel(
		const void *vertexData,
		int vertexCount,
		const Renderer::ShaderSchema *schema)
	{
		// Allocate space for a model.
		Model *model;
		if (!MemoryManager::Allocate(&model)) {
			ErrorStack::Log("Failed to allocate OpenGL model object.\n");
			return nullptr;
		}
		new (model) Model();

		// Initialize the model.
		if (model->Initialize(vertexData, vertexCount, schema)) {
			ErrorStack::Log("Failed to initialize OpenGL model from vertex data.\n");
			return static_cast<Renderer::Model*>(model);
		}

		// Failed to initialize, delete and cleanup.
		MemoryManager::Destroy(model);
		return nullptr;
	}

	// Create a model from a set of vertex and index data.
	Renderer::IndexedModel *Resources::CreateIndexedModel(
		const void *vertexData,
		int vertexCount,
		const unsigned int *indexData,
		int indexCount,
		const Renderer::ShaderSchema *schema)
	{
		// Allocate space for a model.
		IndexedModel *model;
		if (!MemoryManager::Allocate(&model)) {
			ErrorStack::Log("Failed to allocate OpenGL indexed model object.\n");
			return nullptr;
		}
		new (model) Model();

		// Initialize the model.
		if (model->Initialize(vertexData, vertexCount, indexData, indexCount, schema)) {
			ErrorStack::Log("Failed to initialize OpenGL indexed model from vertex/index data.\n");
			return static_cast<Renderer::IndexedModel*>(model);
		}

		// Failed to initialize, delete and cleanup.
		MemoryManager::Destroy(model);
		return nullptr;
	}

	// Destroy a vertex model.
	void Resources::DestroyModel(Renderer::Model *model)
	{
		Model *glModel = static_cast<Model*>(model);
		MemoryManager::Destroy(glModel);
	}

	// Destroy an indexed model.
	void Resources::DestroyIndexedModel(Renderer::IndexedModel *model)
	{
		IndexedModel *glModel = static_cast<IndexedModel*>(model);
		MemoryManager::Destroy(glModel);
	}

	// Create shader from file.
	Renderer::Shader *Resources::CreateShader(const char *filename, Renderer::ShaderType type)
	{
		// Allocate a shader object.
		Shader *glShader;
		if (!MemoryManager::Allocate(&glShader)) {
			ErrorStack::Log("Failed to allocate OpenGL shader object.\n");
			return false;
		}
		new (glShader) Shader();

		// Initialize shader from source.
		if (glShader->Initialize(filename, type)) {
			return static_cast<Renderer::Shader*>(glShader);
		}

		// Initialization failed, clean up.
		MemoryManager::Destroy(glShader);
		return nullptr;
	}

	// Destroy shader.
	// Shader is automatically unlinked.
	void Resources::DestroyShader(Renderer::Shader *shader)
	{
		Shader *glShader = static_cast<Shader*>(shader);
		MemoryManager::Destroy(glShader);
	}

	// Create shader program from vertex/fragment shaders.
	Renderer::Program *Resources::CreateProgram(const Renderer::Shader *vertex, const Renderer::Shader *fragment)
	{
		// Allocate program object.
		Program *program;
		if (!MemoryManager::Allocate(&program)) {
			ErrorStack::Log("Failed to allocate OpenGL shader program.\n", program);
			return nullptr;
		}
		new (program) Program();

		// Initialize with shaders.
		if (program->Initialize(vertex, fragment)) {
			return static_cast<Renderer::Program*>(program);
		}

		// Failed, clean up.
		MemoryManager::Destroy(program);
		return nullptr;
	}

	// Destroy program.
	void Resources::DestroyProgram(Renderer::Program *program)
	{
		Program *glProgram = static_cast<Program*>(program);
		MemoryManager::Destroy(glProgram);
	}

	// Convert generic renderer schema to OpenGL schema.
	Renderer::ShaderSchema *Resources::CreateShaderSchema(
		const Renderer::Program *program,
		const Renderer::Attribute *attributes,
		int attributeCount)
	{
		// Allocate space for the schema.
		ShaderSchema *schema;
		if (!MemoryManager::Allocate(&schema)) {
			ErrorStack::Log("Failed to allocate OpenGL shader schema object.\n");
			return nullptr;
		}

		// Initialize from program and attribute data.
		const Program *glProgram = static_cast<const Program*>(program);
		if (schema->Initialize(glProgram, attributes, attributeCount)) {
			return schema;
		}

		// Failed, clean up.
		MemoryManager::Destroy(schema);
		return nullptr;
	}

	// Clean up shader schema.
	void Resources::DestroyShaderSchema(Renderer::ShaderSchema *schema)
	{
		ShaderSchema *glSchema = static_cast<ShaderSchema*>(schema);
		MemoryManager::Destroy(glSchema);
	}

	// Get a uniform from a program.
	// Program must be bound before this is called.
	Renderer::Uniform *Resources::GetUniform(const Renderer::Program *program, const char *name)
	{
		// Get uniform location.
		const Program *glProgram = static_cast<const Program*>(program);
		GLint location = glProgram->GetUniformLocation(name);
		if (location == -1) {
			ErrorStack::Log("Failed to get location for uniform variable: %s", name);
			return nullptr;
		}

		// Allocate uniform object.
		Uniform *uniform;
		if (!MemoryManager::Allocate(&uniform)) {
			ErrorStack::Log("Failed to allocate OpenGL uniform object.\n");
			return nullptr;
		}
		new (uniform) Uniform(location);
		return uniform;
	}

	// Destroy a uniform variable.
	void Resources::DestroyUniform(Renderer::Uniform *uniform)
	{
		Uniform *glUniform = static_cast<Uniform*>(uniform);
		MemoryManager::Destroy(glUniform);
	}

	/*
	// Create 2D texture from image.
	int opengl_create_texture2d(
		const image_t *image,
		renderer_texture_t *out)
	{
		opengl_texture_t *opengl_texture;
		GLuint texture_handle;

		// Allocate space for the structure.
		opengl_texture = (opengl_texture_t*)memory_allocate(sizeof(opengl_model_t));
		if (opengl_texture == NULL) {
			return 0;
		}
		opengl_null_texture(opengl_texture);
		out->buffer = opengl_texture;

		// Get a texture handle.
		glGenTextures(1, &texture_handle);
		opengl_texture->handle = texture_handle;

		// Bind the texture and load the image data.
		glBindTexture(GL_TEXTURE_2D, texture_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->buffer);

		// Don't tile the textures.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear interpolation for smaller and larger textures.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind the texture.
		glBindTexture(GL_TEXTURE_2D, 0);
		return 1;
	}
	*/

	// Get singleton instance of resource loader.
	Resources *Resources::GetInstance()
	{
		return &instance;
	}

};