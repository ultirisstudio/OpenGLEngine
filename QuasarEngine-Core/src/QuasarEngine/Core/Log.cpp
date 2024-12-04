#include "qepch.h"
#include "Log.h"

#include "PlatformDetection.h"

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <filesystem>

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

namespace QuasarEngine
{
	std::string Log::levelToString(QE_LogLevel level)
	{
		switch (level)
		{
			case QE_INFO: return "INFO";
			case QE_WARNING: return "WARNING";
			case QE_ERROR: return "ERROR";
			case QE_FATAL: return "FATAL";
			default: return "UNKNOWN";
		}
	}

	void Log::LogAPIInfos(const char* vendor, const char* renderer, const char* version)
	{
		const std::string title = "API Infos";

		const std::string versionTitle = "Version: ";
		const std::string vendorTitle = "Vendor: ";
		const std::string rendererTitle = "Renderer: ";

		const std::string versionInfo = static_cast<std::string>(version);
		const std::string vendorInfo = static_cast<std::string>(vendor);
		const std::string rendererInfo = static_cast<std::string>(renderer);

		const size_t ts = title.size();

		const size_t vts = versionTitle.size();
		const size_t vdts = vendorTitle.size();
		const size_t rdts = rendererTitle.size();

		const size_t vis = versionInfo.size();
		const size_t vdis = vendorInfo.size();
		const size_t rdis = rendererInfo.size();

		const size_t max = std::max((vts + vis), std::max((vdts + vdis), (rdts + rdis)));
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
		printInfos(vendorTitle.c_str(), vendorInfo.c_str());
		for (size_t i = 0; i < (max - vdts - vdis) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END VENDOR //

		// RENDERER //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		printInfos(rendererTitle.c_str(), rendererInfo.c_str());
		for (size_t i = 0; i < (max - rdts - rdis) + padding; i++)
			std::cout << " ";
		std::cout << "|" << std::endl;
		// END RENDERER //

		// VERSION //
		std::cout << "|";
		for (size_t i = 0; i < padding; i++)
			std::cout << " ";
		printInfos(versionTitle.c_str(), versionInfo.c_str());
		for (size_t i = 0; i < (max - vts - vis) + padding; i++)
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

	void Log::log(QE_LogLevel level, const std::string& message)
	{
		printLogHeader(level);
		printLog(std::string(message + "\n").c_str());
	}

	void Log::check(bool condition, const char* file, int line, const std::string& message) {
		if (!condition) {
			printLogHeader(QE_FATAL);
			std::stringstream ss;
			ss << message << " from: " << std::filesystem::path(file).filename().string() << " on line : " << line;
			printLog(ss.str().c_str());
			std::exit(1);
		}
	}

	void Log::printLogHeader(QE_LogLevel level)
	{
		std::time_t now = std::time(nullptr);
		std::tm* time = std::localtime(&now);
		std::stringstream ss;
		ss << std::put_time(time, "[%Y-%m-%d %H:%M:%S]");
		ss << " [" << levelToString(level) << "] ";

		const char* str = ss.str().c_str();

		int i = 0;
		while (str[i] != '\0')
		{
			std::cout << YELLOW << str[i] << RESET;
			i++;
		}
	}

	void Log::printInfos(const char* title, const char* info)
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

	void Log::printLog(const char* info)
	{
		int i = 0;
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