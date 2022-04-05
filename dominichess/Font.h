#pragma once

#include "Graphics.h"
#include "Surface2D.h"
#include "Vei2.h"

class Font
{
public:
	Font( const std::string& filename,Color chroma = Colors::White );
	void DrawText( const std::string& text,const Vei2& pos,Color color,Graphics& gfx ) const;
private:
	RectI MapGlyphRect( char c ) const;
private:
	Surface2D surface;
	int glyphWidth;
	int glyphHeight;
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3;
	Color chroma;
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
};