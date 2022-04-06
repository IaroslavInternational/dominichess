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

						break;
					}
				}
			}
		}
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

	size_t rowUp   = row + 1;
	size_t rowDown = row - 1;
	size_t colUp   = col + 1;
	size_t colDown = col - 1;

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
	size_t rrow = EngineFunctions::GenerateRandomNumber<size_t>(5, 7);
	size_t rcol = EngineFunctions::GenerateRandomNumber<size_t>(5, 7);

	if (!bot_figures.empty())
	{
		for (size_t i = 0; i < bot_figures.size(); i++)
		{
			while (bot_figures[i].GetGoal().first == rrow && bot_figures[i].GetGoal().second == rcol)
			{
				rrow = EngineFunctions::GenerateRandomNumber<size_t>(5, 7);
				rcol = EngineFunctions::GenerateRandomNumber<size_t>(5, 7);

				i = 0;
			}	
		}
	}

	return { rrow, rcol };
}

RowAndCol Map::PickRandBotFigure() const
{
	size_t rnum = EngineFunctions::GenerateRandomNumber<size_t>(0, 8);

	return { bot_figures[rnum].GetRow(), bot_figures[rnum].GetCol() };
}
