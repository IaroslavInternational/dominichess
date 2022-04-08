#pragma once

#include "Figure.h"

class BotFigure : public Figure
{
public:
	BotFigure(size_t row, size_t col, const std::string& imgPath, RowAndCol goal, size_t offset_x = 0, size_t offset_y = 0);
public:
	void	  SetGoal(size_t row, size_t col) noexcept;
	RowAndCol GetGoal()	 const noexcept;
	bool      IsOnGoal() const;
	std::pair<short int, short int> GetDistance() const;
private:
	RowAndCol goal;
};

