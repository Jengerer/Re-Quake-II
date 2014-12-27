#pragma once

#include "pack_manager.h"
#include "quake2_common_define.h"

// Public class for managing files for Quake.
class Quake2CommonLibrary QuakeFileManager : public Allocatable
{

public:

	// Singleton initialization and destruction.
	static bool Initialize();
	static void Destroy();

	// Get the singleton instance.
	static QuakeFileManager *GetInstance();

public:

	// Read a file from the pack archive.
	bool Read(const char *filename, FileData *out);

private:

	// Private constructor and destructor for singleton.
	QuakeFileManager();
	~QuakeFileManager();

	// Load available packages.
	bool AddPacks();

public:
	
	// Singleton instance.
	static QuakeFileManager *instance;

private:

	Pack::Manager packs;

};