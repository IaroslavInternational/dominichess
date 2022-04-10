#pragma once

#include "Font.h"

class Graphics;

// Кнопка
class Button
{
public:
	Button(const std::string& txt, size_t width, size_t height, size_t pos_x, size_t pos_y);
public:
	void Draw(Graphics& gfx); // Отрисовка кнопки
public:
	bool IsIntersect(int x, int y); // Проверка вхождения точки в кнопку
	void OnHover();					// Включение подсветки
	void OffHover();				// Выключение подсветки
private:
	size_t width;			  // Ширина кнопки
	size_t height;			  // Высота кнопки
	size_t pos_x;			  // Позиция по оси X
	size_t pos_y;			  // Позиция по оси Y
	bool   IsHovered = false; // Статус наведения мышки
private:
	std::string txt;  // Текст заголовка
	Font        font; // Заголовок
};