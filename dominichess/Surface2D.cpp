#include "Surface2D.h"
#include "EngineWin.h"
#include <cassert>
#include <fstream>
#include <image.hpp>
#include <rgba_pixel.hpp>

Surface2D::Surface2D( std::string filename )
	:
	filename(filename)
{
	if (filename.find(".bmp") != filename.npos)
	{
		std::ifstream file(filename, std::ios::binary);
		assert(file);

		BITMAPFILEHEADER bmFileHeader;
		file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

		BITMAPINFOHEADER bmInfoHeader;
		file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

		assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
		assert(bmInfoHeader.biCompression == BI_RGB);

		const bool is32b = bmInfoHeader.biBitCount == 32;

		width = bmInfoHeader.biWidth;

		int yStart;
		int yEnd;
		int dy;
		if (bmInfoHeader.biHeight < 0)
		{
			height = -bmInfoHeader.biHeight;
			yStart = 0;
			yEnd = height;
			dy = 1;
		}
		else
		{
			height = bmInfoHeader.biHeight;
			yStart = height - 1;
			yEnd = -1;
			dy = -1;
		}

		pPixels = new Color[width * height];

		file.seekg(bmFileHeader.bfOffBits);
		const int padding = (4 - (width * 3) % 4) % 4;

		for (int y = yStart; y != yEnd; y += dy)
		{
			for (int x = 0; x < width; x++)
			{
				PutPixel(x, y, Color(255, file.get(), file.get(), file.get()));
				if (is32b)
				{
					file.seekg(1, std::ios::cur);
				}
			}
			if (!is32b)
			{
				file.seekg(padding, std::ios::cur);
			}
		}
	}
	else if (filename.find(".png") != filename.npos)
	{
		// Изображение
		png::image<png::basic_rgba_pixel<unsigned char>> pic(filename);
		
		// Получить ширину		
		width = pic.get_width();

		// Получить высоту
		height = pic.get_height();

		// Создать массив
		pPixels = new Color[width * height];

		// Заполнить массив
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				auto pixel = pic.get_pixel(j, i);
				PutPixel(j, i, Color(pixel.alpha, pixel.red, pixel.green, pixel.blue));
			}
		}
	}
}

Surface2D::Surface2D( int width,int height )
	:
	width( width ),
	height( height ),
	pPixels( new Color[width*height] )
{
}

Surface2D::Surface2D( const Surface2D& rhs )
	:
	Surface2D( rhs.width,rhs.height )
{
	const int nPixels = width * height;
	for( int i = 0; i < nPixels; i++ )
	{
		pPixels[i] = rhs.pPixels[i];
	}

	filename = rhs.filename;
}

Surface2D::~Surface2D()
{
	delete [] pPixels;
	pPixels = nullptr;
}

Surface2D& Surface2D::operator=( const Surface2D& rhs )
{
	width = rhs.width;
	height = rhs.height;
	filename = rhs.filename;

	delete [] pPixels;
	pPixels = new Color[width*height];

	const int nPixels = width * height;
	for( int i = 0; i < nPixels; i++ )
	{
		pPixels[i] = rhs.pPixels[i];
	}
	return *this;
}

void Surface2D::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	pPixels[y * width + x] = c;
}

Color Surface2D::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	return pPixels[y * width + x];
}

int Surface2D::GetWidth() const
{
	return width;
}

int Surface2D::GetHeight() const
{
	return height;
}

RectI Surface2D::GetRect() const
{
	return{ 0,width,0,height };
}

std::string Surface2D::GetFileName()
{
	return filename;
}
