#include "image.h"
#include "memory_manager.h"
#include "error_stack.h"
#include <png.h>
#include <string.h>

// Constants for image loading.
#define PNG_HEADER_SIZE 8

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

// Private functions for PNG loading.
void png_file_null(png_file_t *png_file);
int png_file_load(const char *filename, png_file_t *png_file);
void png_file_destroy(png_file_t *png_file);

/* Null an image for safe destruction. */
void image_null(image_t *image)
{
	image->buffer = NULL;
	image->width = 0;
	image->height = 0;
	image->channels = 0;
}

/* Initialize an image for a certain size/depth. */
int image_initialize(image_t *image, int width, int height, int channels)
{
	void *buffer;
	int num_pixels;

	// Allocate space.
	num_pixels = width * height;
	buffer = memory_allocate(num_pixels * channels);
	if (buffer == NULL) {
		return 0;
	}

	// Fill out image structure.
	image->buffer = buffer;
	image->width = width;
	image->height = height;
	image->channels = channels;
	return 1;
}

/* Destroy an image. */
void image_destroy(image_t *image)
{
	void *buffer;

	// Free the buffer if it exists.
	buffer = image->buffer;
	if (buffer != NULL) {
		memory_free(buffer);
	}
	image_null(image);
}

/* Load an image from PNG. */
int image_load_png(const char *filename, image_t *out)
{
	png_file_t png_file;
	png_bytepp rows;
	char *buffer;
	int i, row_size;
	int width, height, channels;
	
	// Load the file information.
	png_file_null(&png_file);
	if (!png_file_load(filename, &png_file)) {
		png_file_destroy(&png_file);
		error_stack_log("Failed to load PNG file '%s'.", filename);
		return 0;
	}

	// Convert the information and attempt initialization.
	width = (int)png_file.width;
	height = (int)png_file.height;
	channels = (int)png_file.channels;
	if (!image_initialize(out, width, height, channels)) {
		png_file_destroy(&png_file);
		error_stack_log("Failed to allocate image memory.");
		return 0;
	}

	// Calculate row size.
	row_size = width * channels;

	// Copy the rows into the image buffer.
	buffer = (char*)out->buffer;
	rows = png_file.rows;
	for (i = 0; i < height; ++i) {
		char *row_start;

		// Calculate start position of row.
		row_start = &buffer[i * row_size];
		memcpy(row_start, rows[i], row_size);
	}

	// Clean up.
	png_file_destroy(&png_file);
	return 1;
}

/* Null a PNG file structure for safe deletion. */
void png_file_null(png_file_t *png_file)
{
	png_file->structure = NULL;
	png_file->info = NULL;
	png_file->width = 0;
	png_file->height = 0;
}

/* Load a PNG file and its information into the struct. */
int png_file_load(const char *filename, png_file_t *png_file)
{
	FILE *file;
	png_byte png_header[PNG_HEADER_SIZE];
	png_structp structure;
	png_infop info;

	// Open the file.
	file = fopen(filename, "rb");
	if (file == NULL) {
		error_stack_log("Failed to open file '%s'.", filename);
		return 0;
	}
	png_file->file = file;

	// Verify PNG header.
	if (fread(png_header, PNG_HEADER_SIZE, 1, file) != 1) {
		error_stack_log("Failed to read PNG header for '%s'.", filename);
		return 0;
	}
	if (png_sig_cmp(png_header, 0, PNG_HEADER_SIZE)) {
		error_stack_log("File '%s' was not recognized as PNG.", filename);
		return 0;
	}

	// Prepare loading of PNG.
	structure = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (structure == NULL) {
		error_stack_log("Failed to create PNG read struct.");
		return 0;
	}
	png_file->structure = structure;
	info = png_create_info_struct(structure);
	if (info == NULL) {
		error_stack_log("Failed to create PNG info struct.");
		return 0;
	}
	png_file->info = info;

	// Read the image.
	png_init_io(structure, file);
	png_set_sig_bytes(structure, PNG_HEADER_SIZE);
	png_read_png(structure, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	// Get information.
	png_file->width = png_get_image_width(structure, info);
	png_file->height = png_get_image_height(structure, info);
	png_file->channels = png_get_channels(structure, info);
	png_file->rows = png_get_rows(structure, info);
	return 1;
}

/* Destroy a PNG file structure. */
void png_file_destroy(png_file_t *png_file)
{
	FILE *file;
	png_structp structure;
	png_infop info;

	// Destroy the structure and information; can be NULL.
	structure = png_file->structure;
	info = png_file->info;
	png_destroy_read_struct(&structure, &info, NULL);

	// Close the file if it's open.
	file = png_file->file;
	if (file != NULL) {
		fclose(file);
	}
	png_file_null(png_file);
}
