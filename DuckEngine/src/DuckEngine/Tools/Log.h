#pragma once

#include <iostream>
#include <string>

namespace DuckEngine
{
	class Log
	{
	private:
		static bool firstLog;

		static void beginLog(const std::string& prefix);
		static void endLog();

		static void log()
		{
			endLog();
		}

		template<typename Type>
		static void log(const std::string& prefix, Type value)
		{
			beginLog(prefix);

			std::cout << value;
			log();
		}

		template<typename Type, typename ...TypeList>
		static void log(const std::string& prefix, Type value, TypeList... values)
		{
			beginLog(prefix);

			std::cout << value << ' ';
			log(prefix, values...);
		}

	public:
		Log() = delete;

		template<typename Type, typename ...TypeList>
		static void info(Type value, TypeList... values)
		{
			log("[INFO]: ", value, values...);
		}

		template<typename Type, typename ...TypeList>
		static void debug(Type value, TypeList... values)
		{
			log("[DEBUG]: ", value, values...);
		}

		template<typename Type, typename ...TypeList>
		static void error(Type value, TypeList... values)
		{
			log("[ERROR]: ", value, values...);
		}
	};
}