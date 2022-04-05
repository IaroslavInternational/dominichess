#include "Map.h"
#include "Graphics.h"

Map::Map(size_t offset_x, size_t offset_y)
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

					break;
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
	}

	for (auto& f : figures)
	{
		f.Unselect();
	}
}
