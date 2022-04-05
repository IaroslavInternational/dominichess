#include "App.h"

#include "EngineUtil.h"
#include "EngineFunctions.hpp"

App::App(const std::string& commandLine)
	:
	wnd(std::make_shared<Window>("Domini chess"))
{
	scene = std::make_unique<Scene>("Main Scene", wnd);
	speed_factor = (float)EngineFunctions::GetScreenRefreshTime() / 60.0f;
 }

int App::Go()
{
	// ������� ����
	while( true )
	{
		if(const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		
		const auto dt = timer.Mark() * speed_factor;
		
		HandleInput(dt);
		DoFrame(dt);
	}
}

void App::HandleInput(float dt)
{
	scene->ProcessInput(dt); // ��������� ���������
}

void App::DoFrame(float dt)
{
	wnd->Gfx().BeginFrame(); // ������ �����
	scene->Render(dt);	     // ��������� ���������� �����
	wnd->Gfx().EndFrame();	 // ����� �����
}