#pragma once

#include "Font.h"
#include "Button.h"

#include <memory>

class Graphics;

// Меню
class Menu
{
public:
	Menu(size_t screen_width, size_t screen_height);
public:
	void Draw(Graphics& gfx);		 // Отрисовка меню
	void ProcessHover(int x, int y); // Обработка наведения мыши на кнопки
	int  Click(int x, int y);		 // Обработка клика
	void Toggle()        noexcept;	 // Открытие и закрытие меню
	bool IsShown() const noexcept;   // Статус меню
private:
	size_t offset_x;	  // Отступ по оси X
	size_t offset_y;	  // Отступ по оси Y
	size_t screen_width;  // Ширина окна
	size_t screen_height; // Высота окна
	size_t width;         // Ширина меню
	size_t height;        // Высота меню
private:
	bool IsActive = false; // Статус меню
private:
	std::unique_ptr<Button> cont;               // Кнопка "Продолжение"
	std::unique_ptr<Button> restart;            // Кнопка "Перезагрузка"
	std::unique_ptr<Button> ex;                 // Кнопка "Выход"
	std::vector<std::unique_ptr<Button>*> btns; // Кнопки
private:
	Font title; // Заголовок
};