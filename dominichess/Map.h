#pragma once

#include "Cell.h"
#include "BotFigure.h"
#include "Font.h"

class Graphics;

// Игровое поле
class Map
{
public:
	Map(size_t offset_x, size_t offset_y);
public:
	void Draw(Graphics& gfx);   // Отрисовка поля и фигур
	void Process(int x, int y); // Обработка хода
	void Refresh();				// Перезагрузка
private:
	void		UnSelectAll();								// Убрать выделение
	RowsAndCols GetAvailableSteps(size_t row, size_t col);	// Получить доступные для шага клетки
	bool		IsFigureExists(size_t row, size_t col);		// Проверка существования фигур клетке
	bool		IsBotFigureExists(size_t row, size_t col);	// Проверка существования фигуры бота клетке
private:
	Cell&      GetCell(size_t row, size_t col);			   // Получить клетку по координатам
	Figure&    GetFigure(size_t row, size_t col);		   // Получить фигуру по координатам
	BotFigure& GetBotFigure(size_t row, size_t col);	   // Получить фигуру бота по координатам
	BotFigure& GetBotFigureByGoal(size_t row, size_t col); // Получить фигуру бота по её цели
	void	   SwapGoals(BotFigure& lhs, BotFigure& rhs);  // Поментять местами цели фигуры бота
private:
	void SetTitle(const std::string& txt) noexcept; // Установать заголовок
	void CountScore();								// Вычисление счёта
private:
	RowAndCol CreatePath(const BotFigure& fig);	// Получить путь к цели для фигуры бота
	void	  Optimize();						// Оптимизировать положение фигур бота на победной сетке
private:
	std::vector<Cell>	   cells;							// Клетки
	std::vector<BotFigure> bot_figures;						// Фигуры бота
	std::vector<Figure>	   figures;							// Фигуры игрока
	std::pair<Font, Vei2>  title;							// Заголовок
	std::pair<Font, Vei2>  label_score;						// Надпись с инфорацией об очках
	std::string			   titleTxt  = "Pick your figure!";	// Заголовок
	bool				   IsAITurn  = false;				// Очерёдность хода для бота
	size_t				   AISteps   = 0;					// Кол-во шагов бота
	size_t				   AIScore   = 0;					// Кол-во очков бота
	size_t				   UserScore = 0;					// Кол-во очков игрока
};
