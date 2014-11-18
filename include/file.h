#pragma once

#include "engine_define.h"

// Class for handling file and loading it to a buffer.
class EngineLibrary File
{

public:

	File();
	~File();

	bool Read(const char *filename);
	inline const char *GetBuffer() const;
	inline int GetSize() const;
	
private:

	char *buffer;
	int fileSize;

};

// Get the file buffer.
const char *File::GetBuffer() const
{
	return buffer;
}

// Get the loaded file size.
int File::GetSize() const
{
	return fileSize;
}