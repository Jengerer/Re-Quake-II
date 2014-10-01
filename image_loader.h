#pragma once

#include "image.h"

// Class of image loading functions.
class ImageLoader
{

public:

	// Load a PNG image from file.
	static Image *LoadPNG(const char *filename);

};