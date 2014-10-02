#pragma once

// Class for storing and loading image data.
class Image
{

public:

	Image();

// Image initialization and destruction.

	// Initialie the buffer for a number of pixels and channels.
	bool Initialize(int width, int height, int pixelSize);
	void Destroy();

// Image parameter retrieval.

	// Get the buffer where image data is stored.
	inline void *GetBuffer();
	inline const void *GetBuffer() const;
	inline int GetWidth() const;
	inline int GetHeight() const;
	inline int GetPixelSize() const;

private:

	void *buffer;
	int width, height;
	int pixelSize;

};

// Get the image buffer.
void *Image::GetBuffer()
{
	return buffer;
}

// Get the constant reference image buffer.
const void *Image::GetBuffer() const
{
	return buffer;
}

// Get the image width.
int Image::GetWidth() const
{
	return width;
}

//  Get image height.
int Image::GetHeight() const
{
	return height;
}

// Get number of channels/bytes per pixel.
int Image::GetPixelSize() const
{
	return pixelSize;
}