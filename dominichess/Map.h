#pragma once

#include <vector>
#include "Cell.h"
#include "Figure.h"
#include "Font.h"

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
	void	SetTitle(const std::string& txt) noexcept;
private:
	std::vector<Cell>	  cells;
	std::vector<Figure>	  figures;
	std::pair<Font, Vei2> title;
	std::string			  titleTxt = "Pick your figure!";
};
