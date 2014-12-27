#pragma once

#include "error_stack.h"
#include "pcx_parser.h"
#include "quake_file_manager.h"
#include "wal_parser.h"

namespace WAL
{

	// Colour map file name.
	static const char *ColourMap = "pics/colormap.pcx";

	// WAL generic colour map.
	static Image<PixelRGBA> palette;

	// Initialize the palette from the Quake files.
	bool Parser::LoadPalette()
	{
		// Parse the PCX file but only save its palette.
		QuakeFileManager *quakeFiles = QuakeFileManager::GetInstance();
		Image<PixelRGB> rgbPalette;
		PCX::Parser pcxParser;
		if (!pcxParser.LoadPalette(ColourMap, &rgbPalette)) {
			ErrorStack::Log("Failed to load parse map palette from PCX file.");
			return false;
		}

		// Convert palette to 32-bits.
		int paletteWidth = rgbPalette.GetWidth();
		int paletteHeight = rgbPalette.GetHeight();
		if (!palette.Initialize(paletteWidth, paletteHeight)) {
			ErrorStack::Log("Failed to size 32-bit map palette.");
			return false;
		}
		int pixelCount = paletteWidth * paletteHeight;
		const PixelRGB *inColour = rgbPalette.GetBuffer();
		PixelRGBA *outColour = palette.GetBuffer();

		// Don't set last pixel yet.
		for (int i = pixelCount - 1; i != 0; --i) {
			outColour->r = inColour->r;
			outColour->g = inColour->g;
			outColour->b = inColour->b;
			outColour->a = 255; // All except the last are full opacity.
			outColour++;
			inColour++;
		}

		// Last colour is transparent.
		outColour->r = 0;
		outColour->g = 0;
		outColour->b = 0;
		outColour->a = 0;
		return true;
	}

	// Free the palette buffer.
	void Parser::DestroyPalette()
	{
		palette.Destroy();
	}

	Parser::Parser()
	{
	}

	Parser::~Parser()
	{
	}

	// Parse a WAL file into an image buffer.
	bool Parser::Read(const char *filename, Image<PixelRGBA> *out)
	{
		// Open the WAL file.
		QuakeFileManager *quakeFiles = QuakeFileManager::GetInstance();
		FileData walData;
		if (!quakeFiles->Read(filename, &walData)) {
			return false;
		}
		const Header *header = reinterpret_cast<const Header*>(walData.GetData());

		// Resize output image for data.
		int width = static_cast<int>(header->width);
		int height = static_cast<int>(header->height);
		if (!out->Initialize(width, height)) {
			ErrorStack::Log("Failed to size buffer for WAL image.");	
			return false;
		}

		// Copy data using palette.
		int32_t textureOffset = header->offset[0];
		int pixelCount = width * height;
		const uint8_t *inPixel = reinterpret_cast<const uint8_t*>(walData.GetData() + textureOffset);
		PixelRGBA *outPixel = out->GetBuffer();
		const PixelRGBA *paletteArray = palette.GetBuffer();
		for (int i = 0; i < pixelCount; ++i) {
			*outPixel++ = paletteArray[*inPixel++];
		}
		return true;
	}

}