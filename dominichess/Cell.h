#pragma once

#include "Surface2D.h"

class Graphics;

class Cell
{
public:
	Cell(size_t row, size_t col, const std::string& imgPath, size_t offset_x = 0, size_t offset_y = 0);
public:
	void Draw(Graphics& gfx);
	void CheckOverlapping(int x, int y);
private:
	size_t row;
	size_t col;
	Surface2D image;
	size_t offset_x;
	size_t offset_y;
	bool IsSelected = false;
};
