#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd)
	:
	name(name),
	wnd(wnd)
{
}

/* Методы сцены */

void Scene::ProcessInput(float dt)
{
	/* Обработка переферии */


	/***********************/
}

void Scene::Render(float dt)
{
	/* Отрисовка */

	map.Draw(wnd->Gfx());

	/*************/
}

/****************/