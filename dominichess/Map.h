#pragma once

#include <vector>
#include "Cell.h"
#include "Figure.h"

class Graphics;

using RowsAndCols = std::vector<std::pair<size_t, size_t>>;

class Map
{
public:
	Map(size_t offset_x, size_t offset_y);
public:
	void Draw(Graphics& gfx);
	void DetectObj(int x, int y);
	void Process();
private:
	void		UnSelectAll();
	RowsAndCols GetAvailableSteps(size_t row, size_t col);
	bool		IsFigureExists(size_t row, size_t col);
private:
	Cell&   GetCell(size_t row, size_t col);
	Figure& GetFigure(size_t row, size_t col);
private:
	std::vector<Cell> cells;
	std::vector<Figure> figures;
};

