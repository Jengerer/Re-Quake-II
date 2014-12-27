#pragma once

#include "image.h"
#include "quake_file_manager.h"
#include "quake2_common_define.h"
#include <inttypes.h>

namespace PCX
{

	// PCX file header.
	static const int HeaderPaletteSize = 48;
	static const int ReservedSize2 = 54;
	struct Header
	{
		uint8_t identifier;
		uint8_t version;
		uint8_t encoding;
		uint8_t bitsPerPixel;
		uint16_t startX;
		uint16_t startY;
		uint16_t endX;
		uint16_t endY;
		uint16_t horizontalResolution;
		uint16_t verticalResolution;
		uint8_t palette[HeaderPaletteSize];
		uint8_t reserved1;
		uint8_t bitPlaneCount;
		uint16_t bytesPerLine;
		uint16_t paletteType;
		uint16_t horizontalScreenSize;
		uint16_t verticalScreenSize;
		uint8_t reserved2[ReservedSize2];
	};

	// Class that parses a PCX format file to an image.
	class Quake2CommonLibrary Parser
	{

	public:

		Parser();
		~Parser();

		// Parse data from a PCX file and output to an image.
		bool Load(const char *filename, Image<PixelRGBA> *out);

		// Parse data from a PCX file using a specific palette to an image.
		// TODO: WAL files may be the only ones that use external palette.
		bool Load(const char *filename, const Image<PixelRGB> *palette, Image<PixelRGBA> *out);

		// Load just the palette from the image.
		bool LoadPalette(const char *filename, Image<PixelRGB> *out);

	private:

		// Verify the header matches the expected formats.
		static bool Verify(const Header *header);

	private:

		// Load the file and verify the header.
		bool ReadFile(const char *filename);

		// Get the palette pointer from the data.
		const PixelRGB *GetPaletteData() const;

		// Generic operation of loading the image data using palette data.
		bool LoadHelper(const PixelRGB *palette, Image<PixelRGBA> *out);

	private:

		// Current file data we're parsing.
		FileData imageFile;
		const uint8_t *data;
		int32_t dataLength;
		const Header *header;

	};

}