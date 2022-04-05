#pragma once

#include "EngineTimer.h"
#include "Scene.h"

class App
{
public:
	App(const std::string& commandLine);
public:
	int Go(); // Игровой цикл
private:
	void HandleInput(float dt);	// Обработка данных переферии
	void DoFrame(float dt);		// Отрисовка кадра
private:
	std::shared_ptr<Window>	wnd;   // Указатель на окно
	std::unique_ptr<Scene>	scene; // Текущая сцена
private:
	EngineTimer		timer;				 // Игровой таймер
	float			speed_factor = 1.0f; // Калибровочный параметр скорости
};