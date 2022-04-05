#include "Figure.h"
#include "Graphics.h"

Figure::Figure(size_t row, size_t col, const std::string& imgPath, bool affiliation)
	:
	row(row),
	col(col),
	image(imgPath),
	affiliation(affiliation)
{}

void Figure::Draw(Graphics & gfx)
{
}
