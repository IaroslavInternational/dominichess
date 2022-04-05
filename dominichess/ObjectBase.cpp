#include "ObjectBase.h"

ObjectBase::ObjectBase(size_t row,		size_t col, const std::string& imgPath,
					   size_t offset_x, size_t offset_y)
	:
	row(row),
	col(col),
	offset_x(offset_x),
	offset_y(offset_y),
	image(imgPath)
{}

bool ObjectBase::CheckOverlapping(int x, int y) const
{
	int x1 = offset_x + col * image.GetWidth();
	int y1 = offset_y + row * image.GetHeight();
	int x2 = x1 + image.GetWidth();
	int y2 = y1 + image.GetHeight();

	return (x >= x1 && y >= y1 &&
			x <= x2 && y <= y2);
}

size_t ObjectBase::GetRow() const noexcept
{
	return row;
}

size_t ObjectBase::GetCol() const noexcept
{
	return col;
}

bool ObjectBase::Selected() const noexcept
{
	return IsSelected;
}

void ObjectBase::Select() noexcept
{
	IsSelected = true;
}

void ObjectBase::Unselect() noexcept
{
	IsSelected = false;
}
