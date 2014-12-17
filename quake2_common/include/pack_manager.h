#pragma once

#include "quake2_common_define.h"
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
	class Quake2CommonLibrary Manager
	{

	public:

		Manager();
		~Manager();

		// Load in a new PAK file.
		bool Initialize(const char *filename);

	private:

		File file; // Package file being managed.
		FileData directoryData; // Package directory buffer.

		// Pointer to the directory in the data.
		const Entry *files;
		int32_t fileCount;



	};

}