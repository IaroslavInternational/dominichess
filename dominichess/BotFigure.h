#pragma once

#include "Figure.h"

class BotFigure : public Figure
{
public:
	BotFigure(size_t row, size_t col, const std::string& imgPath, RowAndCol goal, size_t offset_x = 0, size_t offset_y = 0);
public:
	RowAndCol GetGoal() const noexcept;
private:
	RowAndCol goal;
};

