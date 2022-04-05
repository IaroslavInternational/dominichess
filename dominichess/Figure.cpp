#include "Figure.h"
#include "Graphics.h"

Figure::Figure(size_t row, size_t col, const std::string& imgPath, bool IsAI, size_t offset_x, size_t offset_y)
	:
	ObjectBase(row, col, imgPath, offset_x, offset_y),
	IsAI(IsAI)
{}

void Figure::Draw(Graphics& gfx)
{
	gfx.DrawSpriteNonChroma(offset_x + col * image.GetWidth(), offset_y + row * image.GetHeight(), image);
}

bool Figure::IsBot() const noexcept
{
	return IsAI;
}

void Figure::MoveTo(size_t row, size_t col)
{
	this->row = row;
	this->col = col;
}
