#pragma once

#include "ObjectBase.h"

class Figure : public ObjectBase
{
public:
	Figure(size_t row, size_t col, const std::string& imgPath, bool IsAI, size_t offset_x = 0, size_t offset_y = 0);
public:
	void Draw(Graphics& gfx);
	bool IsBot() const noexcept;
	void MoveTo(size_t row, size_t col);
private:
	bool IsAI;
};
