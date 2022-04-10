#include "Scene.h"

Scene::Scene(std::string name, std::shared_ptr<Window> wnd)
	:
	name(name),
	wnd(wnd),
	background("Assets\\Images\\back.png"),
	menu(wnd->Gfx().GetWidth(), wnd->Gfx().GetHeight()),
	map(wnd->Gfx().GetWidth() / 2 - 300, wnd->Gfx().GetHeight() / 2 - 300)
{
}

/* Методы сцены */

void Scene::ProcessInput(float dt)
{
	/* Обработка переферии */

	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			menu.Toggle();
			break;
		default:
			break;
		}
	}
	
	if (!menu.IsShown() && wnd->mouse.LeftIsPressed())
	{
		map.Process(wnd->mouse.GetPosX(), wnd->mouse.GetPosY());
	}
	else
	{
		menu.ProcessHover(wnd->mouse.GetPosX(), wnd->mouse.GetPosY());

		if (wnd->mouse.LeftIsPressed())
		{
			int response = menu.Click(wnd->mouse.GetPosX(), wnd->mouse.GetPosY());

			switch (response)
			{
			case 0:
				break;
			case 1:
				map.Refresh();
				break;
			case -1:
				exit(0);
				break;
			default:
				break;
			}							
		}
	}
	
	/***********************/
}

void Scene::Render(float dt)
{
	/* Отрисовка */
	
	wnd->Gfx().DrawSpriteNonChroma(0, 0, background);

	if (!menu.IsShown())
	{
		map.Draw(wnd->Gfx());
	}

	menu.Draw(wnd->Gfx());

	/*************/
}

/****************/