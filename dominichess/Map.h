#pragma once

#include <vector>
#include "Cell.h"
#include "Figure.h"

class Graphics;

class Map
{
public:
	Map();
public:
	void Draw(Graphics& gfx);
private:
	std::vector<Cell> cells;
	std::vector<Figure> figures;
};

