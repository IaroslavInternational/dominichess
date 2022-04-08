#pragma once

#include <sstream>
#include <random>
#include <Windows.h>

// Специальные функции для движка
namespace EngineFunctions
{
	// Заменить подстроку в строке
	inline std::string static StrReplace(const std::string& inputStr, const std::string& src, const std::string& dst)
	{
		std::string result(inputStr);

		size_t pos = result.find(src);

		while (pos != std::string::npos)
		{
			result.replace(pos, src.size(), dst);
			pos = result.find(src, pos);
		}

		return result;
	}

	template <typename T>
	inline T static GenerateRandomNumber(T min, T max)
	{
		static std::random_device rd;
		static std::mt19937 rng{ rd() };

		if (typeid(T) == typeid(size_t))
		{
			static std::uniform_int_distribution<size_t> ud(min, max);

			return ud(rng);
		}
		else if (typeid(T) == typeid(int))
		{
			static std::uniform_int_distribution<int> ud(min, max);

			return ud(rng);
		}
		else if (typeid(T) == typeid(double))
		{
			static std::uniform_real_distribution<double> ud(min, max);

			return ud(rng);
		}

		return min;
	}

	inline static int IntRandom(int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(min, max);

		return dist(gen);
	}

	inline size_t static GetScreenRefreshTime()
	{
		HDC hDCScreen = GetDC(NULL);
		int RefreshFrequency = GetDeviceCaps(hDCScreen, VREFRESH);

		ReleaseDC(NULL, hDCScreen);

		return size_t(RefreshFrequency);
	}
}