#include "Menu.h"

#include "Graphics.h"

Menu::Menu(size_t screen_width, size_t screen_height)
	:
	screen_width(screen_width),
	screen_height(screen_height),
	title("Assets\\Images\\Fixedsys16x28.bmp")
{
	width  = size_t(screen_width  / 2.5);
	height = size_t(screen_height / 1.7);

	offset_x = size_t((screen_width  - width)  / 2);
	offset_y = size_t((screen_height - height) / 2);

	size_t btn_w = size_t(0.8 * width);
	size_t btn_h = size_t(0.1 * height);
	size_t btn_x = offset_x + size_t((0.2 * width) / 2);
	size_t btn_y = offset_y + size_t(height / 3);

	cont    = std::make_unique<Button>("CONTINUE", btn_w, btn_h, btn_x, btn_y);
	restart = std::make_unique<Button>("RESTART",  btn_w, btn_h, btn_x, btn_y + btn_h + 10);
	ex      = std::make_unique<Button>("EXIT",     btn_w, btn_h, btn_x, btn_y + 4 * btn_h);

	btns.push_back(&cont);
	btns.push_back(&restart);
	btns.push_back(&ex);
}

void Menu::Draw(Graphics& gfx)
{
	if (IsActive)
	{
		gfx.DrawGhostRectangle(offset_x, offset_y, offset_x + width, offset_y + height, Colors::Black, 4.0f);
		
		title.DrawText("DOMINI GAMES", Vei2(offset_x + width / 2 - 96, offset_y + 50), Colors::White, gfx);

		for (auto& b : btns)
		{
			b->get()->Draw(gfx);
		}
	}
}

void Menu::ProcessHover(int x, int y)
{
	for (auto& b : btns)
	{
		b->get()->IsIntersect(x, y) ? b->get()->OnHover() : b->get()->OffHover();
	}
}

int Menu::Click(int x, int y)
{
	if (cont->IsIntersect(x, y))
	{
		IsActive = false;
		return 0;
	}
	else if (restart->IsIntersect(x, y))
	{
		IsActive = false;
		return 1;
	}
	else if (ex->IsIntersect(x, y))
	{
		IsActive = false;
		return -1;		
	}
}

void Menu::Toggle() noexcept
{
	IsActive ? IsActive = false : IsActive = true;
}

bool Menu::IsShown() const noexcept
{
	return IsActive;
}