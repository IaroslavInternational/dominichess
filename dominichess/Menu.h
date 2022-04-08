#pragma once

class Graphics;

class Menu
{
public:
	Menu(size_t screen_width, size_t screen_height);
public:
	void Draw(Graphics& gfx);
private:
	size_t offset_x;
	size_t offset_y;
	size_t screen_width;
	size_t screen_height;
	size_t width;
	size_t height;
};

