#pragma once

#include "EngineTimer.h"
#include "Scene.h"

class App
{
public:
	App(const std::string& commandLine);
public:
	int Go(); // ������� ����
private:
	void HandleInput(float dt);	// ��������� ������ ���������
	void DoFrame(float dt);		// ��������� �����
private:
	std::shared_ptr<Window>	wnd;   // ��������� �� ����
	std::unique_ptr<Scene>	scene; // ������� �����
private:
	EngineTimer		timer;				 // ������� ������
	float			speed_factor = 1.0f; // ������������� �������� ��������
};