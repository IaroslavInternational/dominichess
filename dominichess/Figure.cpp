#include "Figure.h"
#include "Graphics.h"

Figure::Figure(size_t row, size_t col, const std::string& imgPath, bool IsAI, size_t offset_x, size_t offset_y)
	:
	row(row),
	col(col),
	image(imgPath),
	IsAI(IsAI),
	offset_x(offset_x),
	offset_y(offset_y)
{}

void Figure::Draw(Graphics& gfx)
{
	gfx.DrawSpriteNonChroma(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), image);
}

void Figure::CheckOverlapping(int x, int y)
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
