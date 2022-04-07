#pragma once

#include "Cell.h"
#include "BotFigure.h"
#include "Font.h"

class Graphics;

class Map
{
public:
	Map(size_t offset_x, size_t offset_y);
public:
	void Draw(Graphics& gfx);
	void Process(int x, int y);
private:
	void		UnSelectAll();
	RowsAndCols GetAvailableSteps(size_t row, size_t col);
	bool		IsFigureExists(size_t row, size_t col);
private:
	Cell&   GetCell(size_t row, size_t col);
	Figure& GetFigure(size_t row, size_t col);
	void	SetTitle(const std::string& txt) noexcept;
private:
	RowAndCol GenerateGoal();
	RowAndCol CreatePath(const BotFigure& fig);
private:
	std::vector<Cell>	   cells;
	std::vector<BotFigure> bot_figures;
	std::vector<Figure>	   figures;
	std::pair<Font, Vei2>  title;
	std::string			   titleTxt = "Pick your figure!";
	bool				   IsAITurn = false;
};
