#pragma once

#include "opengl_common.h"

namespace OpenGL
{

	// Class representing a renderable object in an OpenGL scene.
	class Model : public Renderer::Model
	{

	public:

		Model();
		~Model();

		// Create the model from a set of vertex data.
		bool Initialize(const void *vertexData, int vertexCount, const Renderer::ShaderSchema *schema);

		// Bind the model for rendering.
		void Bind();

		// Unbind the model from rendering.
		void Unbind();

		// Draw the model.
		void Draw();

	private:

		GLuint vertexBuffer;
		GLsizei vertexCount;

	};

}