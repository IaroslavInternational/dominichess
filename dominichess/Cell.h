#pragma once

#include "Surface2D.h"

class Graphics;

class Cell
{
public:
	Cell(size_t row, size_t col, const std::string& imgPath);
public:
	void Draw(Graphics& gfx);
private:
	size_t row;
	size_t col;
	Surface2D image;
};
