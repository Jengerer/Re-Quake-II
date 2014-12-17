#include "pack_manager.h"
#include <error_stack.h>
#include <string.h>

namespace Pack
{

	// Package magic number.
	static const uint32_t MagicNumber = ('K' << 24) | ('C' << 16) | ('A' << 8) | 'P';

	Directory::Directory(Directory *next) : next(next)
	{
	}

	Directory::~Directory()
	{
	}

	// Initialize package directory from file data.
	bool Directory::Initialize(const char *filename)
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

	// Find an entry in this directory, if one exists.
	// Returns nullptr if no matching filename was found.
	const Entry *Directory::FindEntry(const char *filename) const
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

	// Read a file from the directory by its directory entry.
	bool Directory::Read(const Entry *entry, FileData *out)
	{
		// Seek to the entry and read the file.
		if (!file.Seek(entry->offset, File::OffsetStart)) {
			ErrorStack::Log("Failed to seek to offset %d in pack.", entry->offset);
			return false;
		}

		// Read the entry into the output buffer.
		int32_t entrySize = entry->size;
		if (!out->AllocateData(entrySize)) {
			ErrorStack::Log("Failed to allocate buffer for entry in pack.");
			return false;
		}
		if (!file.Read(entrySize, out)) {
			ErrorStack::Log("Failed to read entry from pack into buffer.");
			return false;
		}
		return true;
	}

	Manager::Manager() : head(nullptr)
	{
	}

	Manager::~Manager()
	{
		// Destroy all directories.
		Directory *directory = head;
		while (directory != nullptr) {
			Directory *next = directory->GetNext();
			delete directory;
			directory = next;
		}
	}

	// Initialize the manager for a specific PAK file.
	bool Manager::AddPack(const char *filename)
	{
		Directory *directory = new Directory(head);
		if (directory == nullptr) {
			ErrorStack::Log("Failed to create directory element.");
			return false;
		}
		if (!directory->Initialize(filename)) {
			delete directory;
			return false;
		}
		head = directory;
		return true;
	}

	// Read a file from the pack.
	// Fills out a file data handle to the file data in the pack.
	bool Manager::Read(const char *filename, FileData *out)
	{
		// Check all the directories for the file.
		for (Directory *directory = head; directory != nullptr; directory = directory->GetNext()) {
			const Entry *entry = directory->FindEntry(filename);
			if (entry != nullptr) {
				if (!directory->Read(entry, out)) {
					ErrorStack::Log("Failed to read file from packs: %s.", filename);
					return false;
				}
				return true;
			}
		}

		// No directory has matching filename.
		ErrorStack::Log("Failed to find file in packs: %s.", filename);
		return false;
	}

}
