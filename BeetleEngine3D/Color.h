#pragma once
#include "ConsolseRenderingEngine.h"

class ColorOperations
{
public:
	struct PixelInfo
	{
		short backgroundColor;
		short foregroundColor;
		wchar_t symbol;
	};

	CHAR_INFO GetColour(float lum)
	{
		int pixelBrightnessIndex = (int)(13.0f * lum);

		static const PixelInfo pixelMapping[] = {
			{ BG_BLACK, FG_BLACK, PIXEL_SOLID },
			{ BG_BLACK, FG_DARK_GREY, PIXEL_QUARTER },
			{ BG_BLACK, FG_DARK_GREY, PIXEL_HALF },
			{ BG_BLACK, FG_DARK_GREY, PIXEL_THREEQUARTERS },
			{ BG_BLACK, FG_DARK_GREY, PIXEL_SOLID },
			{ BG_DARK_GREY, FG_GREY, PIXEL_QUARTER },
			{ BG_DARK_GREY, FG_GREY, PIXEL_HALF },
			{ BG_DARK_GREY, FG_GREY, PIXEL_THREEQUARTERS },
			{ BG_DARK_GREY, FG_GREY, PIXEL_SOLID },
			{ BG_GREY, FG_WHITE, PIXEL_QUARTER },
			{ BG_GREY, FG_WHITE, PIXEL_HALF },
			{ BG_GREY, FG_WHITE, PIXEL_THREEQUARTERS },
			{ BG_GREY, FG_WHITE, PIXEL_SOLID }
		};

		PixelInfo pixelInfo = pixelMapping[pixelBrightnessIndex];
		CHAR_INFO color;
		color.Attributes = pixelInfo.backgroundColor | pixelInfo.foregroundColor;
		color.Char.UnicodeChar = pixelInfo.symbol;
		return color;
	}
};