#include <ShaderGenerator/Symbols/Loader.hpp>

#include <fstream>
#include <sstream>

namespace sg
{
	std::string Loader::readPiece(const std::string& path)
	{
		std::ifstream file(path);
		std::ostringstream stream;

		if (!file.is_open())
			return "";

		stream << file.rdbuf();
		file.close();

		return stream.str();
	}
}
