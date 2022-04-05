#pragma once

#include "Surface2D.h"

class Graphics;

class Figure
{
public:
	Figure(size_t row, size_t col, const std::string& imgPath, bool IsAI);
public:
	void Draw(Graphics& gfx);
private:
	size_t row;
	size_t col;
	Surface2D image;
	bool IsAI;
};
