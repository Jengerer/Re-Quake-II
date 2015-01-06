#include "common.h"

#include <allocatable.h>
#include <image.h>
#include <renderer/texture_interface.h>

namespace OpenGL
{

	// Class implementing a texture in OpenGL.
	class Texture : public Renderer::Texture, public Allocatable
	{

	public:

		Texture();

		// Initialize texture object.
		bool Initialize();

		// Delete this texture resource.
		virtual void Destroy();

		// Bind this texture to a certain slot.
		void Bind(unsigned int textureSlot);

		// Load the texture data from an image.
		bool Load(const Image<PixelRGBA> *image);

	private:

		// Must be deleted through Destroy().
		~Texture();

	private:

		// Handle to OpenGL texture.
		GLuint handle;
	
	};

}