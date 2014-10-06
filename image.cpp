#include "image.h"
#include "memory_manager.h"
#include "error_stack.h"
#include <string.h>

// Empty image constructor.
Image::Image() : buffer(nullptr)
{
}

// Initialize image buffer for a size/depth.
bool Image::Initialize(int width, int height, int pixelSize)
{
	// Allocate space.
	void *buffer;
	int pixelCount = width * height;
	buffer = MemoryManager::Allocate(pixelCount);
	if (buffer == nullptr) {
		return false;
	}

	// Fill out image structure.
	this->buffer = buffer;
	this->width = width;
	this->height = height;
	this->pixelSize = pixelSize;
	return true;
}

// Free the image buffer.
void Image::Destroy()
{
	// Free the buffer if it exists.
	if (buffer != nullptr) {
		MemoryManager::Free(buffer);
	}
}
