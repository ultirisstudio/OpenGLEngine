#include "depch.h"
#include "Log.h"

#include "PlatformDetection.h"

#include <iostream>
#include <algorithm>

#define RESET		"\033[0m"
#define BLACK		"\033[30m"
#define RED			"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define MAGENTA		"\033[35m"
#define CYAN		"\033[36m"
#define WHITE		"\033[37m"
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

namespace OpenGLEngine
{
	void Log::LogOpenGLInfos(const char* vendor, const char* renderer, const char* version)
	{
		const std::string title = "OpenGL Info";

		const std::string glVersionTitle = "Version: ";
		const std::string glVendorTitle = "Vendor: ";
		const std::string glRendererTitle = "Renderer: ";

		const std::string glVersionInfo = static_cast<std::string>(version);
		const std::string glVendorInfo = static_cast<std::string>(vendor);
		const std::string glRendererInfo = static_cast<std::string>(renderer);

		const size_t ts = title.size();

		const size_t glvts = glVersionTitle.size();
		const size_t glvdts = glVendorTitle.size();
		const size_t glrdts = glRendererTitle.size();

		const size_t glvis = glVersionInfo.size();
		const size_t glvdis = glVendorInfo.size();
		const size_t glrdis = glRendererInfo.size();

		const size_t max = std::max((glvts + glvis), std::max((glvdts + glvdis), (glrdts + glrdis)));
		const size_t padding = (66 - max) / 2;

		std::cout << RESET;

		// TOP //
		std::cout << " .";
		for (size_t i = 0; i < (max + padding * 2) - 2; i++)
			std::cout << "-";
		std::cout << "." << std::endl;
		// END TOP //

		// TITLE //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		std::cout << BOLDBLUE << title << RESET;
		for (size_t i = 0; i < (max - ts) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END TITLE //

		// VENDOR //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		PrintInfos(glVendorTitle.c_str(), glVendorInfo.c_str());
		for (size_t i = 0; i < (max - glvdts - glvdis) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END VENDOR //

		// RENDERER //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		PrintInfos(glRendererTitle.c_str(), glRendererInfo.c_str());
		for (size_t i = 0; i < (max - glrdts - glrdis) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END RENDERER //

		// VERSION //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		PrintInfos(glVersionTitle.c_str(), glVersionInfo.c_str());
		for (size_t i = 0; i < (max - glvts - glvis) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END VERSION //

		// BOTTOM //
		std::cout << " '";
		for (size_t i = 0; i < (max + padding * 2) - 2; i++)
			std::cout << "-";
		std::cout << "'" << std::endl;
		// END BOTTOM //
	}

	void Log::PrintInfos(const char* title, const char* info)
	{
		int i = 0;
		while (title[i] != '\0')
		{
			std::cout << YELLOW << title[i] << RESET;
			i++;
		}

		i = 0;
		while (info[i] != '\0')
		{
			switch (info[i])
			{
			case '0':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '1':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '2':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '3':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '4':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '5':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '6':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '7':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '8':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			case '9':
				std::cout << BOLDGREEN << info[i] << RESET;
				break;
			default:
				std::cout << BOLDWHITE << info[i] << RESET;
				break;
			}
			i++;
		}
	}
}