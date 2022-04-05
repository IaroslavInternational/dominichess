#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd)
	:
	name(name),
	wnd(wnd),
	map(wnd->Gfx().GetWidth() / 2 - 300, wnd->Gfx().GetHeight() / 2 - 300)
{
}

/* ������ ����� */

void Scene::ProcessInput(float dt)
{
	/* ��������� ��������� */

	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd->CursorEnabled())
			{
				wnd->DisableCursor();
				wnd->mouse.EnableRaw();
			}
			else
			{
				wnd->EnableCursor();
				wnd->mouse.DisableRaw();
			}
			break;
		default:
			break;
		}
	}

	if (wnd->CursorEnabled())
	{
		if (wnd->mouse.LeftIsPressed())
		{
			map.DetectObj(wnd->mouse.GetPosX(), wnd->mouse.GetPosY());
		}
	}

	/***********************/
}

void Scene::Render(float dt)
{
	/* ��������� */

	map.Draw(wnd->Gfx());

	/*************/
}

/****************/