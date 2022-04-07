#include "Map.h"
#include "Graphics.h"

#include "EngineFunctions.hpp"

Map::Map(size_t offset_x, size_t offset_y)
	:
	title(std::string("Assets\\Images\\Fixedsys16x28.bmp"), {(int)offset_x, (int)offset_y - 50})
{
	{
		bool revert = false;

		// Добавление клеток
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				if (revert)
				{
					cells.emplace_back(i, j, "Assets\\Images\\dark_cell.png", offset_x, offset_y);
				}
				else
				{
					cells.emplace_back(i, j, "Assets\\Images\\light_cell.png", offset_x, offset_y);
				}

				revert = !revert;
			}

			revert = !revert;
		}
	}

	// Добавление фигур бота
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			bot_figures.emplace_back(i, j, "Assets\\Images\\dark_fig.png", GenerateGoal(), offset_x, offset_y);
		}
	}

	// Добавление фигур игрока
	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			figures.emplace_back(i, j, "Assets\\Images\\light_fig.png", offset_x, offset_y);
		}
	}
}

void Map::Draw(Graphics& gfx)
{
	for (auto& c : cells)
	{
		c.Draw(gfx);
	}

	for (auto& f : figures)
	{
		f.Draw(gfx);
	}

	for (auto& bf : bot_figures)
	{
		bf.Draw(gfx);
	}

	title.first.DrawText(titleTxt, title.second, Colors::White, gfx);
}

void Map::Process(int x, int y)
{
	if (!IsAITurn)
	{
		for (auto& c : cells)
		{
			if (c.CheckOverlapping(x, y))
			{
				for (auto& f : figures)
				{
					if (c.GetRow() == f.GetRow() && c.GetCol() == f.GetCol())
					{
						UnSelectAll();

						c.Select();
						f.Select();

						auto steps = GetAvailableSteps(f.GetRow(), f.GetCol());

						for (auto& s : steps)
						{
							GetCell(s.first, s.second).OnStep();
						}

						SetTitle("Make a step!");

						break;
					}
				}

				if (c.IsStepAvailable())
				{
					for (auto& f : figures)
					{
						if (f.Selected())
						{
							f.MoveTo(c.GetRow(), c.GetCol());

							UnSelectAll();

							SetTitle("Mr. Robot's turn!");
							
							IsAITurn = true;
							
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		/* Получение доступных для шага фигур бота */
		
		std::vector<size_t> idx;

		for (size_t i = 0; i < bot_figures.size(); i++)
		{
			if (!bot_figures[i].IsOnGoal())
			{
				if (!GetAvailableSteps(bot_figures[i].GetRow(), bot_figures[i].GetCol()).empty())
				{
					idx.push_back(i);
				}
			}
		}

		/*******************************************/

		/* Выбор случайной фигуры среди доступных */
		
		size_t fig_id = EngineFunctions::GenerateRandomNumber<int>(0, (int)idx.size() - 1);

		/******************************************/

		auto step = CreatePath(bot_figures[fig_id]);
		bot_figures[fig_id].MoveTo(step.first, step.second);

		IsAITurn = false;
	}
}

void Map::UnSelectAll()
{
	for (auto& c : cells)
	{
		c.Unselect();
		c.OffStep();
	}

	for (auto& f : figures)
	{
		f.Unselect();
	}
}

RowsAndCols Map::GetAvailableSteps(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

	RowsAndCols data;

	int rowUp   = row + 1;
	int rowDown = row - 1;
	int colUp   = col + 1;
	int colDown = col - 1;

	if (rowUp >= 0 && rowUp < 8)
	{
		if (!IsFigureExists(rowUp, col))
		{
			data.emplace_back(rowUp, col);
		}
	}

	if (rowDown >= 0 && rowDown < 8)
	{
		if (!IsFigureExists(rowDown, col))
		{
			data.emplace_back(rowDown, col);
		}
	}

	if (colUp >= 0 && colUp < 8)
	{
		if (!IsFigureExists(row, colUp))
		{
			data.emplace_back(row, colUp);
		}
	}

	if (colDown >= 0 && colDown < 8)
	{
		if (!IsFigureExists(row, colDown))
		{
			data.emplace_back(row, colDown);
		}
	}

	return data;
}

bool Map::IsFigureExists(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

	for (auto& f : figures)
	{
		if (f.GetRow() == row && f.GetCol() == col)
		{
			return true;
		}
	}

	for (auto& bf : bot_figures)
	{
		if (bf.GetRow() == row && bf.GetCol() == col)
		{
			return true;
		}
	}

	return false;
}

Cell& Map::GetCell(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

	for (auto& c : cells)
	{
		if (c.GetRow() == row && c.GetCol() == col)
		{
			return c;
		}
	}
}

Figure& Map::GetFigure(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

	for (auto& f : figures)
	{
		if (f.GetRow() == row && f.GetCol() == col)
		{
			return f;
		}
	}
}

void Map::SetTitle(const std::string& txt) noexcept
{
	titleTxt = txt;
}

RowAndCol Map::GenerateGoal()
{
	size_t rrow = EngineFunctions::GenerateRandomNumber<int>(5, 7);
	size_t rcol = EngineFunctions::GenerateRandomNumber<int>(5, 7);

	if (!bot_figures.empty())
	{
		for (size_t i = 0; i < bot_figures.size(); i++)
		{
			while (bot_figures[i].GetGoal().first == rrow && bot_figures[i].GetGoal().second == rcol)
			{
				rrow = EngineFunctions::GenerateRandomNumber<int>(5, 7);
				rcol = EngineFunctions::GenerateRandomNumber<int>(5, 7);

				i = 0;
			}	
		}
	}

	return { rrow, rcol };
}

RowAndCol Map::CreatePath(const BotFigure& fig)
{
	auto steps = GetAvailableSteps(fig.GetRow(), fig.GetCol());

	if (steps.size() == 1)
	{
		return steps[0];
	}

	/*********************************/

	auto dist = fig.GetDistance();

	if (std::abs(dist.first) == 1)
	{
		for (auto& s : steps)
		{
			if (s.first == fig.GetRow() + dist.first && s.second == fig.GetCol())
			{
				return s;
			}
		}
	}
	else if (std::abs(dist.second) == 1)
	{
		for (auto& s : steps)
		{
			if (s.first == fig.GetRow() && s.second == fig.GetCol() + dist.second)
			{
				return s;
			}
		}
	}

	/*********************************/

	if (dist.first > dist.second)
	{
		for (auto& s : steps)
		{
			if (fig.GetGoal().second - fig.GetCol() - s.second < dist.second)
			{
				return s;
			}
		}
	}
	else if (dist.first < dist.second)
	{
		for (auto& s : steps)
		{
			if (fig.GetGoal().first - fig.GetRow() - s.first < dist.first)
			{
				return s;
			}
		}
	}
	else
	{
		std::vector<size_t> idx;

		for (size_t i = 0; i < steps.size(); i++)
		{
			if (fig.GetGoal().second - fig.GetCol() - steps[i].second < dist.second)
			{
				idx.push_back(i);
				return steps[i];
			}
			else if (fig.GetGoal().first - fig.GetRow() - steps[i].first < dist.first)
			{
				idx.push_back(i);
				return steps[i];
			}
		}

		if (!idx.empty())
		{
			auto ID = EngineFunctions::GenerateRandomNumber<int>(0, int(idx.size() - 1));
			return steps[idx[ID]];
		}
	}

	/*********************************/

	size_t stepbacks = 0;

	for (auto& s : steps)
	{
		if (s.first < fig.GetRow() || s.second < fig.GetCol())
		{
			stepbacks++;
		}
	}

	if (stepbacks == steps.size() && stepbacks != 0)
	{
		return steps[EngineFunctions::GenerateRandomNumber<int>(0, stepbacks)];
	}

	/*********************************/

	return steps[EngineFunctions::GenerateRandomNumber<int>(0, steps.size() - 1)];
}