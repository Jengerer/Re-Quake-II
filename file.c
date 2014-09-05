#include "file.h"
#include "memory_manager.h"
#include <stdio.h>

/* Clears a file for safe destruction. */
void file_null(file_t *out)
{
	out->buffer = NULL;
	out->size = 0;
}

/*
 * Read an entire file into memory.
 * Returns 1 on success, 0 otherwise.
 * Caller is responsible for cleaning the returned structure.
 */
int file_load(const char *filename, file_t *out)
{
	FILE *file;
	int length;
	void *buffer;

	// Open the file.
	file = fopen(filename, "rb");
	if (file == NULL) {
		return 0;
	}

	// Get file length and get back to the start.
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return 0;
	}
	length = ftell(file);
	if (length == 0 || (fseek(file, 0, SEEK_SET) != 0)) {
		fclose(file);
		return 0;
	}

	// Allocate space for the file.
	buffer = memory_allocate(length);
	if (buffer == NULL) {
		fclose(file);
		return 0;
	}

	// Update the file structure for safe clean-up.
	out->buffer = buffer;
	out->size = length;

	// Read the file and fill out the buffer.
	if (fread(buffer, length, 1, file) != 1) {
		memory_free(buffer);
		fclose(file);
		return 0;
	}
	fclose(file);

	// Fill output.
	return 1;
}

/* Clean up a file structure. */
void file_destroy(file_t *file)
{
	void *buffer;

	// Free buffer if it has one.
	buffer = file->buffer;
	if (buffer != NULL) {
		memory_free(buffer);
	}
	file_null(file);
}
