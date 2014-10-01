#include "image_loader.h"
#include <stdio.h>

// Structure for loading a PNG file.
typedef struct png_file
{
	FILE *file;
	png_structp structure;
	png_infop info;
	png_uint_32 width;
	png_uint_32 height;
	png_byte channels;
	png_bytepp rows;
} png_file_t;

// Load a PNG image from a file.
Image *ImageLoader::LoadPNG(const char *filename)
{
}