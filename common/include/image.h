#pragma once

#include <allocatable.h>
#include <inttypes.h>
#include <memory_manager.h>

// 24-bit colour pixel.
struct PixelRGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

// 32-bit colour pixel.
struct PixelRGBA
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

// Class for storing and loading image data.
template <typename PixelType>
class Image : public Allocatable
{

public:

	Image();
	~Image();

	// Initialie the buffer for a number of pixels and channels.
	bool Initialize(int width, int height);

	// Destroy the image buffer.
	void Destroy();

	// Get the buffer where image data is stored.
	inline PixelType *GetBuffer() { return pixels; }
	inline const PixelType *GetBuffer() const { return pixels; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline int GetPixelSize() const { return sizeof(PixelType); }

private:

	PixelType *pixels;
	int width, height;

};

template <typename PixelType>
Image<PixelType>::Image() : pixels(nullptr), width(0), height(0)
{
}

template <typename PixelType>
Image<PixelType>::~Image()
{
	Destroy();
}

// Allocate space for the image.
template <typename PixelType>
bool Image<PixelType>::Initialize(int width, int height)
{
	int bufferSize = (width * height) * GetPixelSize();
	pixels = reinterpret_cast<PixelType*>(MemoryManager::Allocate(bufferSize));
	if (pixels == nullptr) {
		ErrorStack::Log("Failed to allocate %d by %d image with %d bytes/pixels.", width, height, GetPixelSize());
		return false;
	}
	this->width = width;
	this->height = height;
	return true;
}

template <typename PixelType>
void Image<PixelType>::Destroy()
{
	if (pixels != nullptr) {
		MemoryManager::Free(pixels);
		pixels = nullptr;
	}
}
