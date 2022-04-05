#include "Figure.h"
#include "Graphics.h"

Figure::Figure(size_t row, size_t col, const std::string& imgPath, bool IsAI)
	:
	row(row),
	col(col),
	image(imgPath),
	IsAI(IsAI)
{}

void Figure::Draw(Graphics & gfx)
{
	gfx.DrawSpriteNonChroma(row * image.GetWidth(), col * image.GetHeight(), image);
}
