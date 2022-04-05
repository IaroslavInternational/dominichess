#include "Map.h"
#include "Graphics.h"

Map::Map()
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
					cells.emplace_back(i, j, "Assets\\Images\\dark_cell.png");
				}
				else
				{
					cells.emplace_back(i, j, "Assets\\Images\\light_cell.png");
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
			figures.emplace_back(i, j, "Assets\\Images\\dark_fig.png", true);
		}
	}

	// Добавление фигур игрока
	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			figures.emplace_back(i, j, "Assets\\Images\\light_fig.png", false);
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
