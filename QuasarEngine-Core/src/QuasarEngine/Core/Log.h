#pragma once

namespace QuasarEngine
{
	class Log
	{
	public:
		enum LogLevel {
			INFO,
			WARNING,
			ERROR,
			FATAL
		};

		static void LogAPIInfos(const char* vendor, const char* renderer, const char* version);
		static void log(LogLevel level, const std::string& message);
		static void assert(bool condition, const char* file, int line, const std::string& message);

	private:
		Log() = delete;
		~Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		static void printLog(const char* info);
		static void printLogHeader(LogLevel level);
		static void printInfos(const char* title, const char* info);
		static std::string levelToString(LogLevel level);
	};

#define Q_ASSERT(condition, message) Log::assert(condition, __FILE__, __LINE__, message)

#define Q_INFO(message) Log::log(Log::INFO, message)
#define Q_WARNING(message) Log::log(Log::WARNING, message)
#define Q_ERROR(message) Log::log(Log::ERROR, message)
#define Q_FATAL(message) Log::log(Log::FATAL, message)
}