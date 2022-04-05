#pragma once

#include <vector>

class Graphics;
class Figure;
class Cell;

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

