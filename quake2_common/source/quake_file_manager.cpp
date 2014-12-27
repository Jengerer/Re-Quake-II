#include "quake_file_manager.h"
#include <error_stack.h>

// Singleton instance reference.
QuakeFileManager *QuakeFileManager::instance;

// Initialize the singleton instance.
bool QuakeFileManager::Initialize()
{
	QuakeFileManager *manager = new QuakeFileManager();
	if (manager == nullptr) {
		ErrorStack::Log("Failed to allocate Quake file manager object.");
		return false;
	}
	if (!manager->Initialize()) {
		delete manager;
		return false;
	}
	QuakeFileManager::instance = manager;
	return true;
}

// Shut down singleton instance.
void QuakeFileManager::Destroy()
{
	if (instance != nullptr) {
		delete instance;
	}
}

// Get the instance of the file manager.
QuakeFileManager *QuakeFileManager::GetInstance()
{
	return instance;
}

// Pack manager for file manager.
bool QuakeFileManager::Read(const char *filename, FileData *out)
{
	if (!packs.Read(filename, out)) {
		return false;
	}
	return true;
}

QuakeFileManager::QuakeFileManager()
{
}

QuakeFileManager::~QuakeFileManager()
{
}

// Add available packages.
bool QuakeFileManager::AddPacks()
{
	const int QuakePackCount = 3;
	const char *QuakePacks[QuakePackCount] = {
		"pak0.pak",
		"pak1.pak",
		"pak2.pak"
	};
	for (int i = 0; i < QuakePackCount; ++i) {
		if (!packs.AddPack(QuakePacks[QuakePackCount])) {
			return false;
		}
	}
	return true;
}
