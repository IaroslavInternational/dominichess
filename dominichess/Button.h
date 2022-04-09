#pragma once

#include "Font.h"

class Graphics;

class Button
{
public:
	Button(const std::string& txt, size_t width, size_t height, size_t pos_x, size_t pos_y);
public:
	void Draw(Graphics& gfx);
public:
	bool IsIntersect(int x, int y);
	void OnHover();
	void OffHover();
private:
	std::string txt;
	size_t width;
	size_t height;
	size_t pos_x;
	size_t pos_y;
	Font font;
	bool IsHovered = false;
};

