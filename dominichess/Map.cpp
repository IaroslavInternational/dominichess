#include "Map.h"
#include "Graphics.h"

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
			figures.emplace_back(i, j, "Assets\\Images\\dark_fig.png", true, offset_x, offset_y);
		}
	}

	// Добавление фигур игрока
	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			figures.emplace_back(i, j, "Assets\\Images\\light_fig.png", false, offset_x, offset_y);
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

	title.first.DrawText(titleTxt, title.second, Colors::White, gfx);
}

void Map::DetectObj(int x, int y)
{
	for (auto& c : cells)
	{
		if (c.CheckOverlapping(x, y))
		{
			for (auto& f : figures)
			{
				if (c.GetRow() == f.GetRow() && c.GetCol() == f.GetCol() && !f.IsBot())
				{
					UnSelectAll();

					c.Select();
					f.Select();

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

						SetTitle("Mr. Bot's turn!");

						break;
					}
				}
			}
		}
	}
}

void Map::Process()
{
	for (auto& f : figures)
	{
		if (!f.IsBot())
		{
			if (f.Selected())
			{
				auto steps = GetAvailableSteps(f.GetRow(), f.GetCol());

				for (auto& s : steps)
				{
					GetCell(s.first, s.second).OnStep();
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
