#include "Cell.h"
#include "Graphics.h"

Cell::Cell(size_t row, size_t col, const std::string& imgPath, size_t offset_x, size_t offset_y)
	:
	ObjectBase(row, col, imgPath, offset_x, offset_y)
{}

void Cell::Draw(Graphics& gfx)
{
	if (IsSelected)
	{
		gfx.DrawSpriteSubstitute(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), Colors::DodgerBlue, image);
		gfx.DrawSpriteGhostNonChroma(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), image);
	}
	else if (IsOnStep)
	{
		gfx.DrawSpriteSubstitute(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), Colors::Green, image);
		gfx.DrawSpriteGhostNonChroma(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), image);
	}
	else
	{
		gfx.DrawSpriteNonChroma(offset_x + row * image.GetWidth(), offset_y + col * image.GetHeight(), image);
	}
}

void Cell::OnStep() noexcept
{
	IsOnStep = true;
}

void Cell::OffStep() noexcept
{
	IsOnStep = false;
}
