#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd)
	:
	name(name),
	wnd(wnd)
{
}

/* ������ ����� */

void Scene::ProcessInput(float dt)
{
	/* ��������� ��������� */


	/***********************/
}

void Scene::Render(float dt)
{
	/* ��������� */

	map.Draw(wnd->Gfx());

	/*************/
}

/****************/