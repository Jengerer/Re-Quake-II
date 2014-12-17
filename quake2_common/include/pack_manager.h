#pragma once

#include <file.h>
#include <inttypes.h>

namespace Pack
{

	// Header structure for a PAK file.
	struct Header
	{
		uint32_t magicNumber;
		int32_t directoryOffset;
		int32_t directorySize;
	};

	// File/directory structure for a PAK file.
	const unsigned int NameLength = 56;
	struct Entry
	{
		uint8_t name[NameLength];
		int32_t offset;
		int32_t size;
	};

	// Package read structure filled out by the manager.
	struct ReadResult
	{
		uint8_t *buffer;
		int32_t size;
	};

	// Class that manages a Quake PAK file directory.
	class PackManager
	{

	public:

		PackManager();
		~PackManager();

		// Load in a new PAK file.
		bool Initialize(const char *filename);

		// Find a file and get the buffer.
		bool Open(const char *filename, ReadResult *out);

	private:

		File file; // Package file being managed.
		FileData directory; // Package directory buffer.

	};

}