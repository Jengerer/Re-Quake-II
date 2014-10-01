#pragma once

// Class for storing and loading image data.
class Image
{

public:

	Image();

// Image initialization and destruction.

	// Initialie the buffer for a number of pixels and channels.
	bool Initialize(int width, int height, int channels);
	void Destroy();

// Image parameter retrieval.

	// Get the buffer where image data is stored.
	inline const void *GetBuffer() const;
	inline int GetWidth() const;
	inline int GetHeight() const;
	inline int GetChannels() const;

private:

	void *buffer;
	int width, height;
	int channels;

};

// Get the image buffer.
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
int Image::GetChannels() const
{
	return channels;
}

// Image loading from certain formats.

int image_load_png(const char *filename, image_t *out);
