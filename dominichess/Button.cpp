#include "Button.h"

#include "Graphics.h"

Button::Button(const std::string& txt, size_t width, size_t height, size_t pos_x, size_t pos_y)
	:
	txt(txt),
	width(width),
	height(height),
	pos_x(pos_x),
	pos_y(pos_y),
	font("Assets\\Images\\Fixedsys16x28.bmp")
{}

void Button::Draw(Graphics& gfx)
{
	if (IsHovered)
	{
		gfx.DrawBorder(pos_x, pos_y, pos_x + width, pos_y + height, Colors::MakeRGB(255u, 237u, 0u));
	}

	font.DrawText(txt, Vei2(pos_x + 10, pos_y + height / 2 - 14), Colors::White, gfx);
}

bool Button::IsIntersect(int x, int y)
{
	return x > pos_x && y > pos_y && x < pos_x + width && y < pos_y + height;
}

void Button::OnHover()
{
	IsHovered = true;
}

void Button::OffHover()
{
	IsHovered = false;
}
