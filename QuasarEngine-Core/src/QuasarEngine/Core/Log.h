#pragma once

namespace QuasarEngine
{
	class Log
	{
	public:
		enum QE_LogLevel {
			QE_INFO,
			QE_WARNING,
			QE_ERROR,
			QE_FATAL
		};

		static void LogAPIInfos(const char* vendor, const char* renderer, const char* version);
		static void log(QE_LogLevel level, const std::string& message);
		static void check(bool condition, const char* file, int line, const std::string& message);

	private:
		Log() = delete;
		~Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		static void printLog(const char* info);
		static void printLogHeader(QE_LogLevel level);
		static void printInfos(const char* title, const char* info);
		static std::string levelToString(QE_LogLevel level);
	};

#define Q_ASSERT(condition, message) Log::check(condition, __FILE__, __LINE__, message)

#define Q_INFO(message) Log::log(Log::QE_INFO, message)
#define Q_WARNING(message) Log::log(Log::QE_WARNING, message)
#define Q_ERROR(message) Log::log(Log::QE_ERROR, message)
#define Q_FATAL(message) Log::log(Log::QE_FATAL, message)

#if defined(DEBUG)
	#define Q_DEBUG(message) Log::log(Log::QE_INFO, message)
#else
	#define Q_DEBUG(message)
#endif
}