#pragma once

namespace OpenGLEngine
{
	class Log
	{
	public:
		static void LogOpenGLInfos(const char* vendor, const char* renderer, const char* version);

	private:
		Log() = delete;
		~Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		static void PrintInfos(const char* title, const char* info);
	};
}