#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Class representing a buffer of data passed to shader.
	class Buffer : public Renderer::Buffer
	{

	public:

		Buffer();
		~Buffer();

		// Create the model from a set of vertex data.
		bool Initialize(
			const void *bufferData,
			int bufferSize,
			const Renderer::BufferSchema *schema);

		// Bind the buffer for rendering.
		void Bind();

		// Unbind the buffer from rendering.
		void Unbind();

		// Draw the model.
		void Draw();

	private:

		GLuint vertexBuffer;
		GLsizei vertexCount;
		GLenum modelType;

	};

}