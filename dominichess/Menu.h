#pragma once

#include "Font.h"
#include "Button.h"
#include <memory>

class Graphics;

class Menu
{
public:
	friend class Map;
public:
	Menu(size_t screen_width, size_t screen_height);
public:
	void Draw(Graphics& gfx);
	void ProcessHover(int x, int y);
	int  Click(int x, int y);
	void Toggle()        noexcept;
	bool IsShown() const noexcept;
private:
	size_t offset_x;
	size_t offset_y;
	size_t screen_width;
	size_t screen_height;
	size_t width;
	size_t height;
private:
	bool IsActive = false;
private:
	std::unique_ptr<Button> cont;
	std::unique_ptr<Button> restart;
	std::unique_ptr<Button> ex;
	std::vector<std::unique_ptr<Button>*> btns;
	Font title;
};