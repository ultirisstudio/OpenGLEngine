#pragma once

#include <iostream>
#include <string.h>
#include <vector>

namespace QuasarEngine
{
	class MyZLib
	{
	public:
		static std::vector<char> compress(const std::vector<char>& str);
		static std::vector<char> decompress(const std::vector<char>& str);
	};
}