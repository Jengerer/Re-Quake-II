#include "file.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Read an entire file into memory.
 * Returns size of file on success and fills the address to allocated buffer.
 * Caller is responsible for cleaning the returned buffer.
 */
int read_file(const char *filename, char **out)
{
	FILE *file;
	int length;
	char *buffer;

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
	buffer = (char*)malloc(length + 1);
	if (buffer == NULL) {
		fclose(file);
		return 0;
	}	

	// Read the file and fill out the pointer.
	if (fread(buffer, length, 1, file) != 1) {
		free(buffer);
		fclose(file);
		return 0;
	}
	fclose(file);
	buffer[length] = '\0';
	*out = buffer;
	return length;
}

