#include "pack_manager.h"
#include <error_stack.h>
#include <string.h>

namespace Pack
{

	// Package magic number.
	const uint32_t MagicNumber = ('K' << 24) | ('C' << 16) | ('A' << 8) | 'P';

	Manager::Manager()
	{
	}

	Manager::~Manager()
	{
	}

	// Initialize the manager for a specific PAK file.
	bool Manager::Initialize(const char *filename)
	{
		if (!file.Open(filename, File::BinaryReadMode)) {
			ErrorStack::Log("Failed to open pack file: %s.", filename);
			return false;
		}

		// Read the header.
		FileData headerData;
		if (!file.Read(sizeof(Header), &headerData)) {
			ErrorStack::Log("Failed to read header for pack file: %s.", filename);
			return false;
		}
		const Header *header = reinterpret_cast<const Header*>(headerData.GetData());
		if (header->magicNumber != MagicNumber) {
			ErrorStack::Log("Invalid package header for pack file: %s.", filename);
			return false;
		}

		// Verify directory size.
		int32_t directorySize = header->directorySize;
		if ((directorySize % sizeof(Entry)) != 0) {
			ErrorStack::Log("Bad directory size for pack file: %s.", filename);
			return false;
		}

		// Read directory.
		if (!file.Seek(header->directoryOffset, File::OffsetStart)) {
			ErrorStack::Log("Failed to seek to directory offset in pack file: %s.", filename);
			return false;
		}
		if (!file.Read(header->directorySize, &directoryData)) {
			ErrorStack::Log("Failed to read directory from pack file: %s.", filename);
			return false;
		}
		files = reinterpret_cast<const Entry*>(directoryData.GetData());
		fileCount = directorySize / sizeof(Entry);
		return true;
	}

	// Read a file from the pack.
	// Fills out a file data handle to the file data in the pack.
	bool Manager::Read(const char *filename, FileData *out)
	{
		const Entry *header = FindHeader(filename);
		if (header == nullptr) {
			ErrorStack::Log("Failed to find file in pack: %s.", filename);
			return false;
		}

		// Seek to the file and load.
		if (!file.Seek(header->offset, File::OffsetStart)) {
			ErrorStack::Log("Failed to seek to file in pack: %s.", filename);
			return false;
		}

		// Read the file.
		if (!file.Read(header->size, out)) {
			ErrorStack::Log("Failed to read file from pack: %s.", filename);
			return false;
		}
		return true;
	}

	// Find the header for a given filename.
	// Returns null if it can't be found.
	const Entry *Manager::FindHeader(const char *filename) const
	{
		const Entry *current = files;
		for (int32_t i = 0; i < fileCount; ++i, ++current) {
			// TODO: make this case insensitive.
			const char *currentName = reinterpret_cast<const char*>(&current->name);
			if (strncmp(filename, currentName, NameLength) == 0) {
				return current;
			}
		}
		return nullptr;
	}


}
