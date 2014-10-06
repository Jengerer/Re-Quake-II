#include "opengl_indexed_model.h"
#include "opengl_shader_schema.h"

namespace OpenGL
{

	// Constructor for empty index model.
	IndexedModel::IndexedModel(Renderer::ModelType modelType) : vertexBuffer(0), indexBuffer(0)
	{
		this->modelType = TranslateModelType(modelType);
	}

	// Destroy model and its buffers.
	IndexedModel::~IndexedModel()
	{
		// Destroy buffers if we have them.
		if (vertexBuffer != 0) {
			glDeleteBuffers(BufferCount, &vertexBuffer);
		}
	}

	// Initialize indexed model from vertex and index data.
	bool IndexedModel::Initialize(
		const void *vertexData,
		int vertexCount, 
		const unsigned int *indexData,
		int indexCount,
		const Renderer::ShaderSchema *schema)
	{
		// Get OpenGL buffers.
		glGenBuffers(BufferCount, &vertexBuffer);

		// Get vertex size.
		// TODO: maybe caller should just pass in buffer size, since it has to add up anyway and they might have it available.
		const ShaderSchema *glSchema = static_cast<const ShaderSchema*>(schema);
		GLsizei vertexSize = glSchema->GetVertexSize();

		// Bind, load vertex/index data, unbind.
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertexData, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
		Unbind();
		return true;
	}

	// Bind vertex/index buffer.
	void IndexedModel::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}

	// Unbind vertex/index buffer.
	void IndexedModel::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Pass the model to the pipeline.
	void IndexedModel::Draw()
	{
		Bind();
		glDrawElements(modelType, indexCount, GL_UNSIGNED_INT, nullptr);
		Unbind();
	}

}