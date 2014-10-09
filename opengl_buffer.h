#pragma once

namespace OpenGL
{

	// Class representing a buffer of data passed to shader.
	class Buffer : public Renderer::Buffer
	{

	public:

		Buffer();
		~Buffer();

		// Create the model from a set of vertex data.
		bool Initialize(const void *bufferData,	int bufferSize);

		// Bind the buffer for rendering.
		void Bind();

		// Unbind the buffer from rendering.
		void Unbind();

	private:

		GLuint vertexBuffer;

	};

}