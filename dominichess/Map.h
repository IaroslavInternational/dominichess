#pragma once

#include <vector>
#include "Cell.h"
#include "Figure.h"

class Graphics;

class Map
{
public:
	Map(size_t offset_x, size_t offset_y);
public:
	void Draw(Graphics& gfx);
	void DetectObj(int x, int y);
private:
	std::vector<Cell> cells;
	std::vector<Figure> figures;
};

