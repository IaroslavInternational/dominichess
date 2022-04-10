#pragma once

#include "Cell.h"
#include "BotFigure.h"
#include "Font.h"

class Graphics;

class Map
{
public:
	friend class Menu;
public:
	Map(size_t offset_x, size_t offset_y);
public:
	void Draw(Graphics& gfx);
	void Process(int x, int y);
	void Refresh();
private:
	void		UnSelectAll();
	RowsAndCols GetAvailableSteps(size_t row, size_t col);
	bool		IsFigureExists(size_t row, size_t col);
	bool		IsBotFigureExists(size_t row, size_t col);
private:
	Cell&      GetCell(size_t row, size_t col);
	Figure&    GetFigure(size_t row, size_t col);
	BotFigure& GetBotFigure(size_t row, size_t col);
	BotFigure& GetBotFigureByGoal(size_t row, size_t col);
	void	   SwapGoals(BotFigure& lhs, BotFigure& rhs);
	void	   SetTitle(const std::string& txt) noexcept;
	void	   CountScore();
private:
	RowAndCol GenerateGoal();
	RowAndCol CreatePath(const BotFigure& fig);
	void	  Optimize();
private:
	std::vector<Cell>	   cells;
	std::vector<BotFigure> bot_figures;
	std::vector<Figure>	   figures;
	std::pair<Font, Vei2>  title;
	std::pair<Font, Vei2>  label_score;
	std::string			   titleTxt  = "Pick your figure!";
	bool				   IsAITurn  = false;
	size_t				   AISteps   = 0;
	size_t				   AIScore   = 0;
	size_t				   UserScore = 0;
};
