#pragma once

#include "quake2_common_define.h"
#include <image.h>
#include <inttypes.h>

namespace WAL
{

	// File header structure.
	static const int NameLength = 32;
	static const int MipMapLevelCount = 4;
	struct Header
	{
		char name[NameLength];
		uint32_t width;
		uint32_t height;
		int32_t offset[MipMapLevelCount]; // Offset in file to the beginning of each level.
		char nextName[NameLength]; // Name of next frame's file.
		int32_t flags;
		int32_t contents;
		int32_t value;
	};

	// WAL parser class.
	class Quake2CommonLibrary Parser
	{

	public:

		// Handle the WAL palette resource.
		static bool LoadPalette();
		static void DestroyPalette();

	public:

		Parser();
		~Parser();

		// Parse a file into an RGBA image using a specific palette.
		bool Read(const char *filename, Image<PixelRGBA> *out);

	};

}