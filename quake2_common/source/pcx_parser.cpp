#include "pcx_parser.h"
#include <error_stack.h>

namespace PCX
{

	// PCX header constants.
	static const uint8_t Identifier = 0x0A;
	static const uint8_t Version = 5;
	static const uint8_t Encoding = 1;
	static const uint8_t BitsPerPixel = 8;

	// 256-color palette parameters.
	static const int PaletteColours = 256;
	static const int PaletteSize = PaletteColours * sizeof(PixelRGB);
	static const int PaletteOffsetEnd = PaletteSize + 1;
	static const int PaletteIdentifier = 0x0C;

	// Data parameters.
	static const uint8_t RunMask = 0xC0;
	static const uint8_t DefaultAlpha = 0xFF;

	Parser::Parser()
	{
	}

	Parser::~Parser()
	{
	}

	// Parse data from a PCX file and output to an image.
	// Returns true on success, false otherwise.
	bool Parser::Load(const char *filename, Image<PixelRGBA> *out)
	{
		if (!ReadFile(filename)) {
			return false;
		}
		const PixelRGB *palette = GetPaletteData();
		if (palette == nullptr) {
			return false;
		}
		if (!LoadHelper(palette, out)) {
			return false;
		}
		return true;
	}

	// Parse just the palette from the file to an image.
	bool Parser::LoadPalette(const char *filename, Image<PixelRGB> *out)
	{
		if (!ReadFile(filename)) {
			return false;
		}

		// Palette is one dimensional.
		if (!out->Initialize(PaletteColours, 1)) {
			ErrorStack::Log("Failed to size palette image.");
			return false;
		}

		// Copy the data over.
		const PixelRGB *inColour = GetPaletteData();
		PixelRGB *outColour = out->GetBuffer();
		for (int i = 0; i < PaletteColours; ++i) {
			*outColour++ = *inColour++;
		}
		return true;
	}

	// Verify the header for the known formats.
	bool Parser::Verify(const Header *header)
	{
		if (header->identifier != Identifier) {
			ErrorStack::Log("Unexpected header identifier for PCX file: %x.", header->identifier);
			return false;
		}
		if (header->version != Version) {
			ErrorStack::Log("Unexpected version for PCX file: %u.", header->version);
			return false;
		}
		if (header->encoding != Encoding) {
			ErrorStack::Log("Unexpected encoding for PCX file: %u.", header->encoding);
			return false;
		}
		if (header->bitsPerPixel != BitsPerPixel) {
			ErrorStack::Log("Unexpected bits per pixel for PCX file: %u.", header->bitsPerPixel);
			return false;
		}
		return true;
	}

	// Read the file and verify the header.
	bool Parser::ReadFile(const char *filename)
	{
		QuakeFileManager *quakeFiles = QuakeFileManager::GetInstance();
		if (!quakeFiles->Read(filename, &imageFile)) {
			return false;
		}
		this->data = imageFile.GetData();
		this->dataLength = imageFile.GetSize();
		header = reinterpret_cast<const Header*>(data);
		if (!Verify(header)) {
			return false;
		}
		return true;
	}

	// Get pointer to 256-colour palette in file data.
	const PixelRGB *Parser::GetPaletteData() const
	{
		// Get the palette being used.
		const uint8_t *paletteStart = data + (dataLength - PaletteOffsetEnd);
		if (*paletteStart != PaletteIdentifier) {
			ErrorStack::Log("No 256-colour palette found at end of file.");
			return nullptr;
		}
		const PixelRGB *palette = reinterpret_cast<const PixelRGB*>(paletteStart + 1);
		return palette;
	}

	// Load the data to the image using a specific palette.
	bool Parser::LoadHelper(const PixelRGB *palette, Image<PixelRGBA> *out)
	{
		// Size output image.
		const uint16_t Width = header->endX - header->startX + 1;
		const uint16_t Height = header->endY - header->startY + 1;
		if (!out->Initialize(Width, Height)) {
			return false;
		}
		const uint8_t *pixelData = reinterpret_cast<const uint8_t*>(header + 1);
		PixelRGBA *outPixel = out->GetBuffer();
		for (uint32_t i = (Width * Height); i != 0; --i) {
			uint8_t current = *pixelData++;
			uint8_t runLength;

			// If run mask is set, we need to write a repeated value.
			if ((current & RunMask) == RunMask) {
				runLength = current & (~RunMask);
				current = *pixelData++;
			}
			else {
				runLength = 1;
			}

			// Write the value out.
			while (runLength != 0) {
				// Get the colour from the palette.
				const PixelRGB *colour = &palette[current];
				outPixel->r = colour->r;
				outPixel->g = colour->g;
				outPixel->b = colour->b;
				outPixel->a = DefaultAlpha;
				--runLength;
				++outPixel;
			}
		}
		return true;
	}

}