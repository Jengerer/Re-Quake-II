#ifndef _IMAGE_H_
#define _IMAGE_H_

/* Structure for storing image data. */
typedef struct image
{
	void *buffer;
	int width;
	int height;
	int channels;
} image_t;

// Image initialization/destruction.
void image_null(image_t *image);
int image_initialize(image_t *image, int width, int height, int channels);
void image_destroy(image_t *image);

// Image loading from certain formats.
int image_load_png(const char *filename, image_t *out);

#endif // _IMAGE_H_