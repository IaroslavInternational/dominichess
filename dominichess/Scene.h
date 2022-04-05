#pragma once

#include "Window.h"

class Scene
{
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd);
public:
	/* ������ ����� */

	void ProcessInput(float dt); // ��������� ������� ������ ���������
	void Render(float dt);		 // ��������� �����

	/****************/
private:
	/* ���������� �������� ����� */
	
	std::string name; // ��� �����

	/*****************************/
private:
	/* ��������������� ���������� */
	
	std::shared_ptr<Window> wnd; // ��������� �� ������� ���� 

	/******************************/
};