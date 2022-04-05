#include "Cell.h"
#include "Graphics.h"

Cell::Cell(size_t row, size_t col, const std::string& imgPath)
	:
	row(row),
	col(col),
	image(imgPath)
{
}

void Cell::Draw(Graphics& gfx)
{
	gfx.DrawSpriteNonChroma(row * image.GetWidth(), col * image.GetHeight(), image);
}
