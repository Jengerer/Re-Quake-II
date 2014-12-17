#include "error_stack.h"
#include "file.h"
#include "memory_manager.h"
#include <stdio.h>

// Start with no buffer/data.
FileData::FileData() : data(nullptr), size(0)
{
}

// Free data if managing any.
FileData::~FileData()
{
	if (data != nullptr) {
		MemoryManager::Free(data);
		data = nullptr;
	}
}

// Allocate space for data to be read from file.
int8_t *FileData::AllocateData(int32_t size)
{
	// Check if we have enough space.
	if (this->size >= size) {
		return data;
	}
	int8_t *buffer = reinterpret_cast<int8_t*>(MemoryManager::Allocate(size));
	if (buffer == nullptr) {
		return false;
	}
	this->data = buffer;
	this->size = size;
	return buffer;
}

// Mode string table.
static const char *ModeStrings[File::OpenModeCount] = {
	"r",
	"rb"
};

File::File() : handle(nullptr)
{
}

// Clean up the file handle.
File::~File()
{
	if (handle != nullptr) {
		fclose(handle);
	}
}

// Open a file for handling.
bool File::Open(const char *filename, OpenMode mode)
{
	// Close previous handle.
	if (handle != nullptr) {
		fclose(handle);
	}
	handle = fopen(filename, ModeStrings[mode]);
	if (handle == nullptr) {
		ErrorStack::Log("Failed to open file: %s\n", filename);
		return false;
	}
	return true;
}

// Get the total size of the file.
int32_t File::GetLength() 
{
	// Get current position to restore.
	int32_t pos = ftell(handle);
	if (pos == -1) {
		ErrorStack::Log("Failed to save current file position when retrieving length.");
		return -1;
	}

	// Seek to end and get offset.
	if (fseek(handle, 0, SEEK_END) != 0) {
		ErrorStack::Log("Failed to seek to end of file to retrieve length.");
		return -1;
	}
	int32_t position = ftell(handle);
	if (position == -1) {
		ErrorStack::Log("Failed to get file seek position to retrieve length.");
		return -1;
	}

	// Seek back to the start.
	if (fseek(handle, pos, SEEK_SET) != 0) {
		ErrorStack::Log("Failed to seek back to original file position.");
		return -1;
	}
	return position;
}

// Seek to a location in the file.
bool File::Seek(int32_t offset, SeekType type)
{
	int translatedType;
	switch (type) {
	case OffsetStart:
		translatedType = SEEK_SET;
		break;
	case OffsetCurrent:
		translatedType = SEEK_CUR;
		break;
	case OffsetEnd:
		translatedType = SEEK_END;
		break;
	default:
		ErrorStack::Log("Bad seek type received.");
		return false;
	}
	if (fseek(handle, offset, translatedType) != 0) { 
		ErrorStack::Log("Failed to seek in file.");
		return false;
	}
	return true;
}

// Read an amount of data from the file.
bool File::Read(int32_t size, FileData *out)
{
	// Allocate an extra byte for text.
	int8_t *data = out->AllocateData(size);
	if (data == nullptr) {
		ErrorStack::Log("Failed to allocate %d bytes to read from file.", size);
		return false;
	}

	// Assume the size was correct.
	size_t count = fread(data, 1, size, handle);
	if (count != size) {
		// If size mismatches, check that it's due to EOF.
		if (feof(handle) == 0) {
			ErrorStack::Log("Failed to read %d bytes from file.", size);
			return false;
		}

		// Update data size to account for reduced size.
		int32_t newSize = static_cast<int32_t>(count);
		out->SetSize(newSize);
	}

	return true;
}

// Read the entire file into memory.
bool File::Read(FileData *out)
{
	// Get file length.
	int32_t length = GetLength();
	if (length == -1) {
		return false;
	}

	// Seek to the start of file.
	if (!Seek(0, OffsetStart)) {
		return false;
	}

	// Perform full size read.
	if (!Read(length, out)) {
		return false;
	}
	return true;
}
