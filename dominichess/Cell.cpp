#include "Cell.h"
#include "Graphics.h"

Cell::Cell(size_t row, size_t col, const std::string& imgPath, size_t offset_x, size_t offset_y)
	:
	row(row),
	col(col),
	image(imgPath),
	offset_x(offset_x),
	offset_y(offset_y)
{
}

void Cell::Draw(Graphics& gfx)
{
	if (IsSelected)
	{
		gfx.DrawSpriteSubstitute(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), Colors::DodgerBlue, image);
	}
	else
	{
		gfx.DrawSpriteNonChroma(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), image);
	}
}

void Cell::CheckOverlapping(int x, int y)
{
	int x1 = offset_x + row * image.GetWidth();
	int y1 = offset_y + col * image.GetHeight();
	int x2 = x1 + image.GetWidth();
	int y2 = y1 + image.GetHeight();

	if (x >= x1 && y >= y1 &&
		x <= x2 && y <= y2)
	{
		IsSelected = true;
	}
}
