#include "opengl_model.h"
#include "opengl_shader_schema.h"

namespace OpenGL
{

	// Constructor for an empty model.
	Model::Model() : vertexBuffer(0)
	{
	}

	// Destroy model and its vertex buffer.
	Model::~Model()
	{
		if (vertexBuffer != 0) {
			glDeleteBuffers(1, &vertexBuffer);
		}
	}

	// Initialize the model for a set of vertex data.
	bool Model::Initialize(const void *vertexData, int vertexCount, const Renderer::ShaderSchema *schema)
	{
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		// TODO: error checking?
		this->vertexBuffer = vertexBuffer;
		this->vertexCount = static_cast<GLuint>(vertexCount);

		// Get size of vertex so properly set buffer size.
		// TODO: maybe caller should just pass in buffer size, since it has to add up anyway and they might have it available.
		const OpenGL::ShaderSchema *glSchema = static_cast<const OpenGL::ShaderSchema*>(schema);
		GLsizei vertexSize = glSchema->GetVertexSize();

		// Bind, load, unbind.
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertexData, GL_STATIC_DRAW);
		Unbind();
	}

	// Bind the model for drawing or loading data.
	void Model::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	}

	// Unbind the model.
	void Model::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Pass the buffer through the pipeline.
	void Model::Draw()
	{
		glDrawElements(GL_TRIANGLES, vertexCount, GL_FLOAT, nullptr);
	}

}