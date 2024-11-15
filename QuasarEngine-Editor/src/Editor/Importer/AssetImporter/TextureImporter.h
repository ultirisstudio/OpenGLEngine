#pragma once

#include <vector>
#include <filesystem>
#include <iostream>

namespace QuasarEngine
{
	class TextureImporter
	{
	public:
		static std::vector<char> Test(std::filesystem::path path)
		{
			std::cout << "texture importer" << std::endl;
			std::vector<char> out;
			out.push_back('a');
			return out;
		}
	};
}