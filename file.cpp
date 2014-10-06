#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include <stdio.h>

// Empty file constructor.
File::File() : buffer(0)
{
}

// Clear file buffer.
File::~File()
{
	if (buffer != nullptr) {
		MemoryManager::DestroyArray(buffer);
	}
}

// Read an entire file into a buffer.
bool File::Read(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (file == nullptr) {
		ErrorStack::Log("Failed to open file: %s\n", filename);
		return false;
	}

	// Get file length and get back to the start.
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		ErrorStack::Log("Failed to seek to end of file: %s\n", filename);
		return false;
	}
	int length = ftell(file);
	if (length == 0 || (fseek(file, 0, SEEK_SET) != 0)) {
		fclose(file);
		ErrorStack::Log("Failed to get length of file: %s\n", filename);
		return false;
	}

	// Allocate space for the file.
	char *buffer;
	if (!MemoryManager::AllocateArray(&buffer, length)) {
		fclose(file);
		ErrorStack::Log("Failed to allocate space for file: %s\n", filename);
		return false;
	}
	this->buffer = buffer;

	// Read the file and fill out the buffer.
	if (fread(buffer, length, 1, file) != 1) {
		fclose(file);
		return false;
	}
	fclose(file);
	fileSize = length;
	return true;
}
