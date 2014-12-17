#pragma once

#include "common_define.h"
#include <inttypes.h>
#include <stdio.h>

// Class for managing data returned from a file.
class CommonLibrary FileData
{

public:

	FileData();
	~FileData();

	// Allocate the buffer for the data.
	uint8_t *AllocateData(int32_t size);	

	// Change the reported data size (when reading text, file size and text size differ).
	inline void SetSize(int32_t size) { this->size = size; }

	// Retrieve file parameters.
	inline const uint8_t *GetData() const { return data; }
	inline int32_t GetSize() const { return size; }


private:

	uint8_t *data;
	int32_t size;

};

// Class for handling file and loading it to a buffer.
class CommonLibrary File
{

public:
	
	// Enum for seek parameter.
	enum SeekType
	{
		OffsetStart, // Go to exact location from start of file.
		OffsetCurrent, // Move relative to the current file position.
		OffsetEnd // Move relative to end of the file.
	};

	// Enum for file open type.
	// Indices are fixed to map to the mode translation table.
	enum OpenMode
	{
		ReadMode = 0,
		BinaryReadMode = 1,
		OpenModeCount
	};

public:

	File();
	~File();

	// Set the file to open.
	bool Open(const char *filename, OpenMode mode);

	// Get the full length of the file.
	int32_t GetLength();

	// Seek to an offset in the file.
	bool Seek(int32_t offset, SeekType type);

	// Read an amount of data from the file.
	bool Read(int32_t size, FileData *out);

	// Read the whole file into memory.
	bool Read(FileData *out);

private:

	// Implementing file handle.
	FILE *handle;

};
