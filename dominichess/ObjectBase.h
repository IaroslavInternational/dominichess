#pragma once

#include <vector>
#include "Surface2D.h"

using RowAndCol   = std::pair<size_t, size_t>;
using RowsAndCols = std::vector<RowAndCol>;

class Graphics;

class ObjectBase
{
public:
	ObjectBase(size_t row,          size_t col, const std::string& imgPath, 
		       size_t offset_x = 0, size_t offset_y = 0);
	virtual ~ObjectBase() {};
public:
	virtual void Draw(Graphics& gfx) = 0;
	bool         CheckOverlapping(int x, int y) const;
public:
	size_t GetRow()   const noexcept;
	size_t GetCol()   const noexcept;
	bool   Selected() const noexcept;
	void   Select()         noexcept;
	void   Unselect()       noexcept;
protected:
	size_t    row;
	size_t    col;
	size_t    offset_x;
	size_t    offset_y;
	Surface2D image;
	bool      IsSelected = false;
};
