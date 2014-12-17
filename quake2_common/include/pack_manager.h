#pragma once

#include "quake2_common_define.h"
#include <allocatable.h>
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

	// Class for representing a pack directory.
	class Directory : public Allocatable
	{

	public:

		Directory(Directory *directory);
		~Directory();

		// Initialize pack directory from file data.
		bool Initialize(const char *filename);

		// Get a reference to a file in the pack.
		// Returns nullptr if the file does not exist.
		const Entry *FindEntry(const char *filename) const;

		// Load a file from the pack by its header.
		bool Read(const Entry *entry, FileData *out);
		
		// Intrusive list functions.
		inline Directory *GetNext() { return next; }

	private:

		File file; // Package file being managed.
		FileData directoryData; // Package directory buffer.

		// Pack directory members.
		const Entry *files;
		int32_t fileCount;

		// Intrusive list elements.
		Directory *next;

	};

	// Class that manages a Quake PAK file directory.
	class Quake2CommonLibrary Manager
	{

	public:

		Manager();
		~Manager();

		// Load in a new PAK file.
		bool AddPack(const char *filename);

		// Read a file from the pack.
		// Fills out a file data handle to the file data from the pack.
		bool Read(const char *filename, FileData *out);

	private:

		Directory *head;

	};

}