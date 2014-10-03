#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Class representing a renderable object with indices referencing the vertices.
	class IndexedModel : public Renderer::IndexedModel
	{

	public:

		IndexedModel();
		~IndexedModel();

		// Create the model from a set of vertex and index data.
		bool Initialize(
			const void *vertexData,
			int vertexCount,
			const unsigned int *indexData,
			int indexCount,
			const Renderer::ShaderSchema *schema);

		// Bind the model for rendering.
		void Bind();

		// Unbind model from rendering.
		void Unbind();

		// Send model data to pipeline.
		void Draw();

	private:

		// Helper value for initialization/shutdown.
		static const int BufferCount = 2;

		// Buffer objects.
		GLuint vertexBuffer, indexBuffer;
		GLsizei indexCount;
	
	};

}