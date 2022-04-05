#pragma once

#include "Window.h"

class Scene
{
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd);
public:
	/* Методы сцены */

	void ProcessInput(float dt); // Обработка входных данных переферии
	void Render(float dt);		 // Отрисовка сцены

	/****************/
private:
	/* Переменные описания сцены */
	
	std::string name; // Имя сцены

	/*****************************/
private:
	/* Вспомогательные переменные */
	
	std::shared_ptr<Window> wnd; // Указатель на главное окно 

	/******************************/
};