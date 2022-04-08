#include "Menu.h"

#include "Graphics.h"

Menu::Menu(size_t screen_width, size_t screen_height)
	:
	screen_width(screen_width),
	screen_height(screen_height)
{
	width  = size_t(screen_width  / 3);
	height = size_t(screen_height / 1.3);

	offset_x = size_t((screen_width  - width)  / 2);
	offset_y = size_t((screen_height - height) / 2);
}

void Menu::Draw(Graphics& gfx)
{
	gfx.DrawGhostRectangle(offset_x, offset_y, offset_x + width, offset_y + height, Colors::Black, 3.0f);
}
