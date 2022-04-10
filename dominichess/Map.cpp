#include "Map.h"
#include "Graphics.h"

#include "EngineFunctions.hpp"

Map::Map(size_t offset_x, size_t offset_y)
	:
	title(std::string("Assets\\Images\\Fixedsys16x28.bmp"),       {(int)offset_x, (int)offset_y - 50}),
	label_score(std::string("Assets\\Images\\Fixedsys16x28.bmp"), {(int)offset_x, (int)offset_y + 625})
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
			bot_figures.emplace_back(i, j, "Assets\\Images\\dark_fig.png", RowAndCol({ i + 5, j + 5 }), offset_x, offset_y);
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

	std::ostringstream oss;
	oss << "Your score: " << UserScore << " | Mr. Robot's score: " << AIScore;

	title.first.DrawText(titleTxt, title.second, Colors::White, gfx);
	label_score.first.DrawText(oss.str(), label_score.second, Colors::White, gfx);
}

void Map::Process(int x, int y)
{
	if (UserScore == 9)
	{
		titleTxt = "You won!";
		return;
	}
	else if (AIScore == 9)
	{
		titleTxt = "You've lost!";
		return;
	}

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
							CountScore();
							
							break;
						}
					}
				}
			}
		}
	}
	
	if(IsAITurn && AIScore != 9)
	{
		if (AISteps != 0 && AISteps % 4 == 0 && AIScore >= 3)
		{
			Optimize();
			CountScore();

			AISteps++;

			return;
		}

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
		
		size_t fig_id = idx[EngineFunctions::IntRandom(0, (int)idx.size() - 1)];

		/******************************************/

		auto step = CreatePath(bot_figures[fig_id]);
		bot_figures[fig_id].MoveTo(step.first, step.second);

		AISteps++;
		IsAITurn = false;

		SetTitle("Make a step!");
		CountScore();
	}
}

void Map::Refresh()
{
	UnSelectAll();

	size_t id = 0;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			bot_figures[id].MoveTo(i, j);
			bot_figures[id].SetGoal(i + 5, j + 5);

			id++;
		}
	}

	id = 0;

	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			figures[id].MoveTo(i, j);

			id++;
		}
	}

	AISteps   = 0;
	AIScore   = 0;
	UserScore = 0;
	IsAITurn  = false;
	titleTxt  = "Pick your figure!";
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

bool Map::IsBotFigureExists(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

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

BotFigure& Map::GetBotFigure(size_t row, size_t col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(row < 8);
	assert(col < 8);

	for (auto& bf : bot_figures)
	{
		if (bf.GetRow() == row && bf.GetCol() == col)
		{
			return bf;
		}
	}
}

BotFigure& Map::GetBotFigureByGoal(size_t row, size_t col)
{
	for (auto& bf : bot_figures)
	{
		if (bf.GetGoal().first == row && bf.GetGoal().second == col)
		{
			return bf;
		}
	}
}

void Map::SwapGoals(BotFigure& lhs, BotFigure& rhs)
{
	auto g = lhs.GetGoal();

	lhs.SetGoal(rhs.GetGoal().first, rhs.GetGoal().second);
	rhs.SetGoal(g.first, g.second);
}

void Map::SetTitle(const std::string& txt) noexcept
{
	titleTxt = txt;
}

void Map::CountScore()
{
	AIScore   = 0;
	UserScore = 0;

	for (auto& bf : bot_figures)
	{
		if (bf.GetRow() > 4 && bf.GetCol() > 4)
		{
			AIScore++;
		}
	}

	for (auto& f : figures)
	{
		if (f.GetRow() < 3 && f.GetCol() < 3)
		{
			UserScore++;
		}
	}
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
			if (fig.GetGoal().second - s.second < dist.second)
			{
				return s;
			}
		}
	}
	else if (dist.first < dist.second)
	{
		for (auto& s : steps)
		{
			if (fig.GetGoal().first - s.first < dist.first)
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
			if (fig.GetGoal().second - steps[i].second < dist.second)
			{
				idx.push_back(i);
			}
			else if (fig.GetGoal().first - steps[i].first < dist.first)
			{
				idx.push_back(i);
			}
		}

		if (!idx.empty())
		{
			return steps[EngineFunctions::IntRandom(0, int(idx.size() - 1))];
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
		return steps[EngineFunctions::IntRandom(0, stepbacks - 1)];
	}

	/*********************************/

	return steps[EngineFunctions::IntRandom(0, steps.size() - 1)];
}

void Map::Optimize()
{
	bool grid[3][3];

	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			grid[i - 5][j - 5] = IsBotFigureExists(i, j);
		}
	}

	for (size_t i = 5; i < 8; i++)
	{
		for (size_t j = 5; j < 8; j++)
		{
			if (grid[i - 5][j - 5] && i + 1 < 8)
			{
				if (!grid[i - 4][j - 5])
				{
					SwapGoals(GetBotFigure(i, j), GetBotFigureByGoal(i + 1, j));
					GetBotFigure(i, j).MoveTo(i + 1, j);

					return;
				}
			}
			
			if (grid[i - 5][j - 5] && j + 1 < 8) 
			{			
				if (!grid[i - 5][j - 4])
				{
					SwapGoals(GetBotFigure(i, j), GetBotFigureByGoal(i, j + 1));
					GetBotFigure(i, j).MoveTo(i, j + 1);

					return;
				}
			}
		}
	}
}
