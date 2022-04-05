#pragma once

#include "ObjectBase.h"

class Cell : public ObjectBase
{
public:
	Cell(size_t row, size_t col, const std::string& imgPath, size_t offset_x = 0, size_t offset_y = 0);
public:
	void Draw(Graphics& gfx);
public:
	void OnStep()   noexcept;
	void OffStep()  noexcept;
	bool IsStepAvailable() const noexcept;
private:
	bool IsOnStep = false;
};
