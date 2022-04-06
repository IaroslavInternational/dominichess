#include "BotFigure.h"

BotFigure::BotFigure(size_t row, size_t col, const std::string& imgPath, RowAndCol goal, size_t offset_x, size_t offset_y)
	:
	Figure(row, col, imgPath, offset_x, offset_y),
	goal(goal)
{}

RowAndCol BotFigure::GetGoal() const noexcept
{
	return goal;
}
