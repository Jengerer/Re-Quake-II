#include "texture.h"
#include <error_stack.h>

namespace OpenGL
{

	Texture::Texture() : handle(0)
	{
	}

	Texture::~Texture()
	{
		if (handle != 0) {
			glDeleteTextures(1, &handle);
		}
	}

	// Create the texture handle.
	bool Texture::Initialize()
	{
		glGenTextures(1, &handle);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to generate OpenGL texture handle.");
			return false;
		}
		return true;
	}

	// Bind this texture to a texture slot.
	void Texture::Bind(unsigned int textureSlot)
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	// Load the texture data from an image.
	bool Texture::Load(const Image<PixelRGBA> *image)
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to bind texture to load image.");
			return false;
		}

		// Set up texture parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Pass in the data from the image.
		glTexImage2D(
			GL_TEXTURE_2D,
			0, // Level-of-detail number.
			GL_RGBA,
			image->GetWidth(),
			image->GetHeight(),
			0, // Border.
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image->GetBuffer());
		if (glGetError() != GL_NO_ERROR) {
			ErrorStack::Log("Failed to load image data into texture.");
			return false;
		}
		return true;
	}

}
