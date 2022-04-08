#include "BotFigure.h"

BotFigure::BotFigure(size_t row, size_t col, const std::string& imgPath, RowAndCol goal, size_t offset_x, size_t offset_y)
	:
	Figure(row, col, imgPath, offset_x, offset_y),
	goal(goal)
{}

void BotFigure::SetGoal(size_t row, size_t col) noexcept
{
	goal.first  = row;
	goal.second = col;
}

RowAndCol BotFigure::GetGoal() const noexcept
{
	return goal;
}

bool BotFigure::IsOnGoal() const
{
	return row == goal.first && col == goal.second;
}

std::pair<short int, short int> BotFigure::GetDistance() const
{
	return { goal.first - row, goal.second - col };
}
